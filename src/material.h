#ifndef MATERIAL_H
#define MATERIAL_H

#include<QString>
#include<QHash>

class Material
{
public:
    bool spheremap;
    bool aomap;
    bool stagemap;
    bool cubemap;
    bool diffuse;
    bool diffuse2;
    bool diffuse3;
    bool ramp;
    bool shadow;
    bool dummyramp;
    bool normalmap;

    int textureCount = 0;
    int expectedTextureCount = 0;

    int srcFactor;
    int dstFactor;

	int cullMode;

	int alphaFunc;
	int alphaTest;

    QString fileName;
    QHash<QString, QList<float>> properties;
	uint flags;

    enum TextureFlags {
        Glow = 0x00000080,
        Shadow = 0x00000040,
        DummyRamp = 0x00000020,
        SphereMap = 0x00000010,
        StageAOMap = 0x00000008,
        RampCubeMap = 0x00000004,
        NormalMap = 0x00000002,
        DiffuseMap = 0x00000001
    };

    void testFlags();

    void calculateTextureCount();
};

#endif // MATERIAL_H
