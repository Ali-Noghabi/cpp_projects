#include "trianglerenderer.h"

typedef void *EGLDisplay;

TriangleRenderer::TriangleRenderer(QObject *parent)
    : QObject(parent), m_offscreenSurface(new QOffscreenSurface) {
    m_offscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_offscreenSurface->create();



    m_context = new QOpenGLContext;
    m_context->shareContext();

    m_context->setFormat(m_offscreenSurface->format());
    m_context->create();
    m_context->makeCurrent(m_offscreenSurface);

}

void TriangleRenderer::renderTriangle(const QSize &size)
{
    m_framebuffer.reset(new QOpenGLFramebufferObject(size));
    m_framebuffer->bind();


    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawTriangle(size.width(), size.height());

    m_context->functions()->glFlush();
    m_framebuffer->release();

    saveToPNG();

}

void TriangleRenderer::drawTriangle(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // glBegin(GL_TRIANGLES);

    // glColor3f(1, 0, 0); // Red
    // glVertex2f(0, height); // Bottom-left

    // glColor3f(0, 1, 0); // Green
    // glVertex2f(width / 2, 0); // Top-center

    // glColor3f(0, 0, 1); // Blue
    // glVertex2f(width, height); // Bottom-right

    // glEnd();

    // glBegin(GL_QUADS);

    // glColor3f(1, 0, 0); // Red
    // glVertex2f(0.0, 0.0); // Top

    // glColor3f(0, 1, 0); // Green
    // glVertex2f(10.0,0.0); // Right

    // glColor3f(0, 0, 1); // Blue
    // glVertex2f(10.0,10.0); // Left

    // glColor3f(1, 1, 0); // Blue
    // glVertex2f(0.0,10.0); // Left

    // glEnd();

    // int gridSize = 10;
    // glPushMatrix();
    // glTranslatef(0, 0, 0); // move to the top-left corner of the grid
    // for (int row = 0; row < gridSize; row++) {
    //     for (int col = 0; col < gridSize; col++) {
    //         glBegin(GL_QUADS);

    //         // Set color based on position for demonstration
    //         if ((row + col) % 2 == 0) {
    //             glColor3f(1, 1, 1); // White for even position
    //         } else {
    //             glColor3f(0, 0, 0); // Black for odd position
    //         }

    //         glVertex2f(col * 10, row * 10); // Top-left
    //         glVertex2f((col + 1) * 10, row * 10); // Top-right
    //         glVertex2f((col + 1) * 10, (row + 1) * 10); // Bottom-right
    //         glVertex2f(col * 10, (row + 1) * 10); // Bottom-left

    //         glEnd();
    //     }
    // }
    // glPopMatrix();

    QList<QJsonArray> Y;
    QList<QJsonArray> color;

     Y.append( QJsonArray({0,0.5}));
     Y.append( QJsonArray({0,0.8}));
     Y.append( QJsonArray({0,1.1}));
      color.append( QJsonArray({"#FF0000","#FF0000"}));
      color.append( QJsonArray({"#00FF00","#00FF00"}));
      color.append( QJsonArray({"#0000FF","#0000FF"}));

    // Y.append( QJsonArray({0,0.5,0,0,0,0}));
    // Y.append( QJsonArray({1,1.5,1,1,1,1} ));
    // Y.append( QJsonArray({1,1.5,1,1,1,1} ));
    // Y.append( QJsonArray({2,2,2,2,2,2} ));
    // Y.append( QJsonArray({3,3,3,3,3,3} ));
    // Y.append( QJsonArray({3,3,3,3,3,3} ));
    // Y.append( QJsonArray({4,4,4,4,4,4} ));

    // color.append( QJsonArray({"#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000"}));
    // color.append( QJsonArray({"#00FF00","#00FF00","#00FF00","#00FF00","#00FF00","#00FF00"} ));
    // color.append( QJsonArray({"#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF"} ));
    // color.append( QJsonArray({"#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00"} ));
    // color.append( QJsonArray({"#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00"} ));
    // color.append( QJsonArray({"#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00","#FFFF00"} ));



    int gridSizeX = Y.first().size();
    int gridSizeY = Y.size();

    double cellWidth = static_cast<double>(width) / gridSizeX;
    double cellHeight = static_cast<double>(height) / gridSizeY;

    // for (int row = 0; row < gridSizeY; row++) {
    //     for (int col = 0; col < gridSizeX; col++) {
    //         QJsonArray rowData = Y[row];
    //         QJsonArray colorData = color[row];

    //         int value = rowData[col].toInt();
    //         QColor cellColor = QColor::fromString(colorData[col].toString());

    //         glBegin(GL_QUADS);
    //         glColor3f(cellColor.redF(), cellColor.greenF(), cellColor.blueF());
    //         glVertex2f(col * cellWidth, row * cellHeight); // Top-left
    //         glVertex2f((col + 1) * cellWidth, row * cellHeight); // Top-right
    //         glVertex2f((col + 1) * cellWidth, (row + 1) * cellHeight); // Bottom-right
    //         glVertex2f(col * cellWidth, (row + 1) * cellHeight); // Bottom-left
    //         glEnd();
    //     }
    // }

    // int gridSizeX = sizeX;
    // int gridSizeY = Y.size();

    double stepX = static_cast<double>(width) / gridSizeX;
    double stepY = static_cast<double>(height) / gridSizeY;

    for (int row = 0; row < gridSizeY - 1; row++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int col = 0; col < gridSizeX; col++) {
            float value1 = Y[row][col].toDouble();
            float value2 = Y[row + 1][col].toDouble();
            QColor cellColor(color[row][col].toString());
            glColor3f(cellColor.redF(), cellColor.greenF(), cellColor.blueF());
            glVertex3f(col * stepX, (row * stepY)+(value1*stepY), 0);

            QColor cellColor2(color[row + 1][col].toString());
            glColor3f(cellColor2.redF(), cellColor2.greenF(), cellColor2.blueF());

            glVertex3f(col * stepX, ((row + 1) * stepY)+ (value2*stepY), 0);
        }
        glEnd();
    }
}

void TriangleRenderer::saveToPNG()
{
    QImage img = m_framebuffer->toImage(false);
    img.save("triangle.png");
}
