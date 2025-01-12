/* Copyright 2025 Petr Mrázek
 *
 * This source is subject to the Microsoft Permissive License (MS-PL).
 * Please see the COPYING.md file for more information.
 */

#pragma once

#include <QtGui/QOpenGLFunctions>

#include <QVector3D>
#include <vector>
#include <memory>

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "SkinTypes.h"

namespace Skins {

#pragma pack(push, 0)

struct Vertex {
    Vertex(const QVector3D & position, float u, float v, GLuint texture, bool transparency)
    {
        m_position[0] = position.x();
        m_position[1] = position.y();
        m_position[2] = position.z();
        m_uv[0] = u;
        m_uv[1] = v;
        m_texture = texture;
        m_transparency = transparency;
    }

    GLfloat m_position[3];
    GLfloat m_uv[2];
    GLint m_texture; // 0 = skin, 1 = cape
    GLint m_transparency; // 0 = opaque, 1 = transparent
};
#pragma pack(pop)

class Uniform
{
public:
    Uniform(QString name): name(name) {}
    QString name;
    int id = -1;
};

class RenderContext : public QObject
{
    Q_OBJECT
public:
    explicit RenderContext(QOpenGLFunctions& GL, QObject* parent);
    void init();
    void deinit();

    void setTextures(const QImage& skin, const QImage& cape);

    void regenerateGeometry(int skinVersion, Model skinModel);
    void render(QMatrix4x4 viewMatrix, QVector3D baseColor, QVector3D alternateColor, float devicePixelRatio);

    QOpenGLTexture* m_skinTexture = nullptr;
    QOpenGLTexture* m_capeTexture = nullptr;

    std::vector<Vertex> m_vertexBuffer;
    std::vector<GLuint> m_elementBuffer;
    unsigned int m_elementStartIndex;

    QOpenGLVertexArrayObject *m_vao = nullptr;
    QOpenGLBuffer* m_vbo = nullptr;
    QOpenGLBuffer* m_ebo = nullptr;

    QOpenGLVertexArrayObject *m_bgvao = nullptr;
    QOpenGLBuffer* m_bgvbo = nullptr;
    QOpenGLBuffer* m_bgebo = nullptr;

    QOpenGLShaderProgram* m_skinShader = nullptr;
    QList<Uniform> m_uniforms;

    QOpenGLShaderProgram* m_bgShader = nullptr;
    QList<Uniform> m_bgUniforms;

    QOpenGLFunctions& GL;
private:
    bool initShader(QOpenGLShaderProgram* program, QList<Uniform>& uniforms, const QString& vertexShaderPath, const QString& fragmentShaderPath);
};

}
