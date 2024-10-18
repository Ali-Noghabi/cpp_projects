#include "RenderOffScreen.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <array>
#include <fstream>
#include <iostream>
#include <png.h>
#include <vector>

#include <QColor>
#include <QImage>
#include <QJsonArray>
#include <QString>
#include <QtNumeric>
#include <QtAlgorithms>
//! Boundary Field
static const double  xMin = 222892.7;
static const double  xMax = 225740.44;
static const double  yMin = 3452922.9;
static const double  yMax = 3464106.7;

static QColor  interpolateColor(double value)
{
    // Ensure value is within [0.0, 1.0] range
    value = qBound(0.0, value, 1.0);

    // Define gradient colors
    QColor  startColor(Qt::red);
    QColor  middleColor(Qt::green);
    QColor  endColor(Qt::blue);

    // Determine the position within the gradient
    double  position = value * 2.0;

    if (position < 1.0)
    {
        // Interpolate between red and green
        return QColor(
            startColor.red()
            + static_cast<int>((middleColor.red() - startColor.red()) * position),
            startColor.green()
            + static_cast<int>((middleColor.green() - startColor.green())
                               * position),
            startColor.blue()
            + static_cast<int>((middleColor.blue() - startColor.blue())
                               * position));
    }
    else
    {
        // Interpolate between green and blue
        position -= 1.0;

        return QColor(
            middleColor.red()
            + static_cast<int>((endColor.red() - middleColor.red()) * position),
            middleColor.green()
            + static_cast<int>((endColor.green() - middleColor.green())
                               * position),
            middleColor.blue()
            + static_cast<int>((endColor.blue() - middleColor.blue())
                               * position));
    }
}

// Define the Vertex structure
struct Vertex
{
    GLfloat  x, y, z;    // Position
    GLfloat  r, g, b, a; // Color
};

// Function to convert a hex color string to RGBA floats
void  hexToRgba(const QString &hex, float &r, float &g, float &b, float &a)
{
    QColor  color(hex);
    r = color.redF();
    g = color.greenF();
    b = color.blueF();
    a = color.alphaF();
}

static const EGLint  configAttribs[] = { EGL_SURFACE_TYPE,
                                           EGL_PBUFFER_BIT,
                                           EGL_BLUE_SIZE,
                                           8,
                                           EGL_GREEN_SIZE,
                                           8,
                                           EGL_RED_SIZE,
                                           8,
                                           EGL_DEPTH_SIZE,
                                           8,
                                           EGL_RENDERABLE_TYPE,
                                           EGL_OPENGL_ES2_BIT,
                                           EGL_NONE };

static EGLint        pbufferAttribs[] = { EGL_WIDTH, 800, EGL_HEIGHT, 600, EGL_NONE };
static const EGLint  contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2,
                                            EGL_NONE };

#define STRINGIFY(x) #x
// Vertex Shader with position and color attributes
static const char * vertexShaderCode = STRINGIFY(attribute vec3 pos;
                                                 attribute vec4 vertexColor; // New attribute for the vertex color
                                                 varying vec4 fragColor;     // Pass color to the fragment shader

                                                 void    main()
{
    gl_Position = vec4(pos, 1.0);
    fragColor   = vertexColor; // Pass the color to the fragment
});

// Fragment Shader using interpolated color
static const char * fragmentShaderCode = STRINGIFY(precision mediump float;
                                                   varying vec4 fragColor; // Received from the vertex shader

                                                   void  main()
{
    gl_FragColor = fragColor; // Use the color passed from the
});

static const char* eglGetErrorStr()
{
    switch (eglGetError())
    {
    case EGL_SUCCESS:
        return "The last function succeeded without error.";
    case EGL_NOT_INITIALIZED:
        return "EGL is not initialized, or could not be initialized, for the "
               "specified EGL display connection.";
    case EGL_BAD_ACCESS:
        return "EGL cannot access a requested resource (for example a context "
               "is bound in another thread).";
    case EGL_BAD_ALLOC:
        return "EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE:
        return "An unrecognized attribute or attribute value was passed in the "
               "attribute list.";
    case EGL_BAD_CONTEXT:
        return "An EGLContext argument does not name a valid EGL rendering "
               "context.";
    case EGL_BAD_CONFIG:
        return "An EGLConfig argument does not name a valid EGL frame buffer "
               "configuration.";
    case EGL_BAD_CURRENT_SURFACE:
        return "The current surface of the calling thread is a window, pixel "
               "buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY:
        return "An EGLDisplay argument does not name a valid EGL display "
               "connection.";
    case EGL_BAD_SURFACE:
        return "An EGLSurface argument does not name a valid surface (window, "
               "pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH:
        return "Arguments are inconsistent (for example, a valid context "
               "requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER:
        return "One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP:
        return "A NativePixmapType argument does not refer to a valid native "
               "pixmap.";
    case EGL_BAD_NATIVE_WINDOW:
        return "A NativeWindowType argument does not refer to a valid native "
               "window.";
    case EGL_CONTEXT_LOST:
        return "A power management event has occurred. The application must "
               "destroy all contexts and reinitialise OpenGL ES state and "
               "objects to continue rendering.";
    default:
        break;
    }

    return "Unknown error!";
}

RenderOffScreen::RenderOffScreen(QObject *parent):
    QObject(parent)
{
    m_colors.push_back(Qt::blue);
    m_colors.push_back(Qt::yellow);
    m_colors.push_back(Qt::green);
    m_colors.push_back(Qt::magenta);
    m_colors.push_back(Qt::red);
    m_colorStops.push_back(0.0);
    m_colorStops.push_back(0.25);
    m_colorStops.push_back(0.5);
    m_colorStops.push_back(0.75);
    m_colorStops.push_back(1.0);
}

RenderOffScreen::~RenderOffScreen()
{
}

static bool  sortByFirstElement(const QPair<QJsonArray, QJsonArray> &a, const QPair<QJsonArray, QJsonArray> &b)
{
    return a.first.at(0).toDouble() < b.first.at(0).toDouble();
}

void  RenderOffScreen::setData(QList<QJsonArray> &data,
                               QList<QJsonArray> &colorValues)
{
    m_data.clear();
    m_value.clear();

    if ((data.size() < 2) || (data.size() != colorValues.size()))
    {
        return;
    }

    QList<QPair<QJsonArray, QJsonArray>>  tempContainer;

    for (int i = 0; i < data.size(); ++i)
    {
        tempContainer.append(qMakePair(data[i], colorValues[i]));
    }

    // Sort
    std::sort(tempContainer.begin(), tempContainer.end(), sortByFirstElement);
    data.clear();
    colorValues.clear();

    for (int i = 0; i < tempContainer.size(); ++i)
    {
        data.append(tempContainer[i].first);
        colorValues.append(tempContainer[i].second);
    }

    // find minimum and maximum y value
    double  min = (double)INTMAX_MAX;
    double  max = (double)INTMAX_MIN;

    for (auto var : data)
    {
        for (int i = 0; i < var.size(); ++i)
        {
            auto  value = var[i].toDouble();

            min = qMin(min, value);
            max = qMax(max, value);
        }
    }

    double  cmin = (double)INTMAX_MAX;
    double  cmax = (double)INTMAX_MIN;

    for (auto var : colorValues)
    {
        for (int i = 0; i < var.size(); ++i)
        {
            auto  value = var[i].toDouble();

            cmin = qMin(cmin, value);
            cmax = qMax(cmax, value);
        }
    }

    m_minColor = cmin;
    m_maxColor = cmax;

    auto                                 diffrence = max - min;
    auto                                 origin    = min + (diffrence / 2);
    QList<QList<QPair<double, double>>>  matrix;

    for (int i = 0; i < data.size(); ++i)
    {
        double                        interval = (double)2 / (double)(data[i].size() - 1);
        double                        x        = (double)-1;
        QList<QPair<double, double>>  row;

        for (int j = 0; j < data[i].size(); ++j)
        {
            row.append(qMakePair(x, (origin - data[i][j].toDouble()) / (diffrence / 2)));
            x += interval;
        }

        matrix.append(row);
    }

    for (int i = 0; i < matrix.size() - 1; ++i)
    {
        for (int j = 0; j < matrix[i].size() - 1; ++j)
        {
            QJsonArray  jarray;
            QJsonArray  carray;

            jarray.append(matrix[i][j].first);
            jarray.append(matrix[i][j].second);
            m_data.append(jarray);
            carray.append(colorValues[i][j]);
            carray.append(colorValues[i][j]);
            m_value.append(carray);

            jarray = QJsonArray();
            jarray.append(matrix[i + 1][j].first);
            jarray.append(matrix[i + 1][j].second);
            m_data.append(jarray);
            carray = QJsonArray();
            carray.append(colorValues[i + 1][j]);
            carray.append(colorValues[i + 1][j]);
            m_value.append(carray);

            jarray = QJsonArray();
            jarray.append(matrix[i + 1][j + 1].first);
            jarray.append(matrix[i + 1][j + 1].second);
            m_data.append(jarray);
            carray = QJsonArray();
            carray.append(colorValues[i + 1][j + 1]);
            carray.append(colorValues[i + 1][j + 1]);
            m_value.append(carray);

            jarray = QJsonArray();
            jarray.append(matrix[i][j].first);
            jarray.append(matrix[i][j].second);
            m_data.append(jarray);
            carray = QJsonArray();
            carray.append(colorValues[i][j]);
            carray.append(colorValues[i][j]);
            m_value.append(carray);

            jarray = QJsonArray();
            jarray.append(matrix[i + 1][j + 1].first);
            jarray.append(matrix[i + 1][j + 1].second);
            m_data.append(jarray);
            carray = QJsonArray();
            carray.append(colorValues[i + 1][j + 1]);
            carray.append(colorValues[i + 1][j + 1]);
            m_value.append(carray);

            jarray = QJsonArray();
            jarray.append(matrix[i][j + 1].first);
            jarray.append(matrix[i][j + 1].second);
            m_data.append(jarray);
            carray = QJsonArray();
            carray.append(colorValues[i][j + 1]);
            carray.append(colorValues[i][j + 1]);
            m_value.append(carray);
        }
    }

    // m_data.append(data[0]);
    // m_data.append(data[1]);
    // m_data.append(data[2]);

    ////! Get Maximum value from color values
    ////! double             maxValue = 0.224849;
    ////!
    ////! QColor color =  interpolateColor(colorValues[X]/maxValue)
    ////!

    // m_value.append(colorValues[0]);
    // m_value.append(colorValues[1]);
    // m_value.append(colorValues[2]);

    // for (int i = 3; i < lenght; ++i)
    // {
    // m_data.append(data[i - 2]);
    // m_data.append(data[i - 1]);
    // m_data.append(data[i]);
    // m_value.append(colorValues[i - 2]);
    // m_value.append(colorValues[i - 1]);
    // m_value.append(colorValues[i]);
    // }
}

void  RenderOffScreen::setData2(QList<QJsonArray> &data, QList<QJsonArray> &colorValues)
{
    m_data  = data;
    m_value = colorValues;
}

QImage  RenderOffScreen::render(const QSize &size)
{
    if (m_data.isEmpty())
    {
        return QImage();
    }

    EGLDisplay  display;
    int         major, minor;
    int         desiredWidth, desiredHeight;
    GLuint      program, vert, frag;

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Failed to get EGL display! Error: %s\n", eglGetErrorStr());
    }

    if (eglInitialize(display, &major, &minor) == EGL_FALSE)
    {
        fprintf(stderr, "Failed to get EGL version! Error: %s\n", eglGetErrorStr());
        eglTerminate(display);
    }

    printf("Initialized EGL version: %d.%d\n", major, minor);

    EGLint     numConfigs;
    EGLConfig  config;

    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs))
    {
        fprintf(stderr, "Failed to get EGL config! Error: %s\n", eglGetErrorStr());
        eglTerminate(display);
    }

    pbufferAttribs[1] = size.width();
    pbufferAttribs[3] = size.height();

    EGLSurface  surface = eglCreatePbufferSurface(display, config, pbufferAttribs);

    if (surface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Failed to create EGL surface! Error: %s\n",
                eglGetErrorStr());
        eglTerminate(display);
    }

    eglBindAPI(EGL_OPENGL_API);

    EGLContext  context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    if (context == EGL_NO_CONTEXT)
    {
        fprintf(stderr, "Failed to create EGL context! Error: %s\n",
                eglGetErrorStr());
        eglDestroySurface(display, surface);
        eglTerminate(display);
    }

    eglMakeCurrent(display, surface, surface, context);

    std::vector<Vertex>  vertices;
    int                  gridSizeX = m_data.first().size();
    int                  gridSizeY = m_data.size();
    double               stepX     = static_cast<double>(size.width()) / gridSizeX;
    double               stepY     = static_cast<double>(size.height()) / gridSizeY;

    // Populate the vertices data...
    // Assuming Y and color are filled as in your provided snippet
    for (int i = 0; i < m_data.size(); ++i)
    {
        QJsonArray  point = m_data[i];
        QJsonArray  col   = m_value[i];
        float       r, g, b, a = 1.0;

        getColorFromGradient(col[0].toDouble() / m_maxColor, r, g, b);
        // hexToRgba(col[0].toString(), r, g, b, a);

        // Assuming Z coordinate is 0.0 for all vertices and converting 2D points to
        // 3D
        vertices.push_back({ static_cast<float>(point[0].toDouble()),
                             static_cast<float>(point[1].toDouble()), 0.0f, r, g, b,
                             a });
    }

    // The desired width and height is defined inside of pbufferAttribs
    // Check top of this file for EGL_WIDTH and EGL_HEIGHT
    desiredWidth  = size.width();
    desiredHeight = size.height();

    // Set GL Viewport size, always needed!
    glViewport(0, 0, desiredWidth, desiredHeight);

    // Get GL Viewport size and test if it is correct.
    // NOTE! DO NOT UPDATE EGL LIBRARY ON RASPBERRY PI AS IT WILL INSTALL FAKE
    // EGL! If you have fake/faulty EGL library, the glViewport and
    // glGetIntegerv won't work! The following piece of code checks if the gl
    // functions are working as intended!
    GLint  viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    // viewport[2] and viewport[3] are viewport width and height respectively
    printf("GL Viewport size: %dx%d\n", viewport[2], viewport[3]);

    // Test if the desired width and height match the one returned by
    // glGetIntegerv
    if ((desiredWidth != viewport[2]) || (desiredHeight != viewport[3]))
    {
        fprintf(stderr, "Error! The glViewport/glGetIntegerv are not working! "
                        "EGL might be faulty!\n");
    }

    // Clear whole screen (front buffer)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create a shader program
    // NO ERRRO CHECKING IS DONE! (for the purpose of this example)
    // Read an OpenGL tutorial to properly implement shader creation
    program = glCreateProgram();

    GLuint  vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    glUseProgram(program);
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderCode, NULL);
    glCompileShader(vert);
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderCode, NULL);
    glCompileShader(frag);
    glAttachShader(program, frag);
    glAttachShader(program, vert);
    glLinkProgram(program);
    glUseProgram(program);
    glUseProgram(program);

    // Get attribute locations
    GLint  posLoc   = glGetAttribLocation(program, "pos");
    GLint  colorLoc = glGetAttribLocation(program, "vertexColor");

    // Enable vertex attribute arrays
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(colorLoc);

    // Set attribute pointers
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)0);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(3 * sizeof(float)));

    // Draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    // Create buffer to hold entire front buffer pixels
    // We multiply width and height by 3 to because we use RGB!
    unsigned char *buffer = (unsigned char *)malloc(desiredWidth * desiredHeight * 4);

    // Copy entire screen
    glReadPixels(0, 0, desiredWidth, desiredHeight, GL_RGBA, GL_UNSIGNED_BYTE,
                 buffer);

    QImage  image(desiredWidth, desiredHeight, QImage::Format_RGBA8888);

    memcpy(image.bits(), buffer, desiredWidth * desiredHeight * 4);

    // Free copied pixels
    free(buffer);

    // Cleanup
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    glDisableVertexAttribArray(posLoc);
    glDisableVertexAttribArray(colorLoc);

    return image;
}

QList<QColor>  RenderOffScreen::colors() const
{
    return m_colors;
}

void  RenderOffScreen::setColors(const QList<QColor> &newColors)
{
    m_colors = newColors;
}

QList<qreal>  RenderOffScreen::colorStops() const
{
    return m_colorStops;
}

void  RenderOffScreen::setColorStops(const QList<qreal> &newColorStops)
{
    m_colorStops = newColorStops;
}

void  RenderOffScreen::getColorFromGradient(double value, GLfloat &r, GLfloat &g, GLfloat &b)
{
    if ((m_colorStops.size() != m_colors.size()) || m_colorStops.isEmpty())
    {
        return;
    }

    // Normalize value to be within the range of the stops
    qreal  t = (value - m_colorStops.first()) / (m_colorStops.last() - m_colorStops.first());

    if (t < 0)
    {
        t = 0;
    }
    else if (t > 1)
    {
        t = 1;
    }

    // Find the segment in which the value falls
    int  i;

    for (i = 0; i < m_colorStops.size() - 1; ++i)
    {
        if (t <= m_colorStops[i + 1])
        {
            break;
        }
    }

    // Compute the interpolated color
    qreal  delta = (t - m_colorStops[i]) / (m_colorStops[i + 1] - m_colorStops[i]);

    r = m_colors[i].redF() + delta * (m_colors[i + 1].redF() - m_colors[i].redF());
    g = m_colors[i].greenF() + delta * (m_colors[i + 1].greenF() - m_colors[i].greenF());
    b = m_colors[i].blueF() + delta * (m_colors[i + 1].blueF() - m_colors[i].blueF());
}
