#ifndef TRIANGLERENDERER_H
#define TRIANGLERENDERER_H

#include <QObject>
#include <QtOpenGL>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>

class TriangleRenderer : public QObject

{
    Q_OBJECT
public:
    explicit TriangleRenderer(QObject *parent = nullptr);

    void renderTriangle(const QSize &size);

private:
    void drawTriangle(int width, int height);
    void saveToPNG();


    QOffscreenSurface *m_offscreenSurface;
    QOpenGLContext *m_context;
    QScopedPointer<QOpenGLFramebufferObject> m_framebuffer;
};

#endif // TRIANGLERENDERER_H
