#ifndef NEVO_RENDER_PIPELINE_H
#define NEVO_RENDER_PIPELINE_H

#include "NevoVec.h"

namespace nme
{

class Surface;

}

namespace nevo
{

class VBO;
class EBO;

class Job
{
public:
    Job();
    ~Job();
    
    void mtl(nme::Surface *surface, unsigned int color, int blendMode);
    bool cmpmtl(Job *job);
    void rect(float x, float y, float width, float height);
    void tile(float x, float y, int rectX, int rectY, int rectW, int rectH, float *inTrans);
    void triangles(int inXYs_n, float *inXYs,
        int inIndixes_n, short *inIndixes, int inUVT_n, float *inUVT,
        int inColours_n, int *inColours);
    bool hitTest(float x, float y);
    void clear();

    void setTypeRect() { mFlags |= 1; }
    void setTypeTile() { mFlags |= 2; }
    void setTypeTriangles() { mFlags |= 4; }
    bool isTypeRect() { return mFlags & 1; }
    bool isTypeTile() { return mFlags & 2; }
    bool isTypeTriangles() { return mFlags & 4; }

    void setBlendModeNone() { mFlags |= 8; }
    void setBlendModeNormal() { mFlags |= 16; }
    void setBlendModeAdd() { mFlags |= 32; }
    bool isBlendModeNone() { return mFlags & 8; }
    bool isBlendModeNormal() { return mFlags & 16; }
    bool isBlendModeAdd() { return mFlags & 32; }
    unsigned char getBlendMode() { return mFlags & 56; }

    float r() { return mR / 255.0f; }
    float g() { return mG / 255.0f; }
    float b() { return mB / 255.0f; }
    float a() { return mA / 255.0f; }

public:
    nme::Surface *mSurface;
    union {
        unsigned int mBGRA;
        struct { unsigned char mB, mG, mR, mA; };
    };
    unsigned char mFlags;

    union
    {
        struct
        {
            VBO *mT_XY;
            VBO *mT_UV;
            VBO *mT_C;
            EBO *mT_I;
            int mT_In;
            float mT_BBminX, mT_BBminY, mT_BBmaxX, mT_BBmaxY;
        };
        
        struct
        {
            struct { float x, y; } mQ_XY[4];
            struct { float u, v; } mQ_UV[4];
        };
    };
};

class JobsPool
{
public:
    JobsPool();
    ~JobsPool();

    Job* get();
    void refund(Job *job);
private:
    Vec<Job*> mAlloc;
    Vec<Job*> mFree;
};

class NevoRenderPipeline
{
public:
    NevoRenderPipeline();
    ~NevoRenderPipeline();

    void Init();
    void Clear();

    void setJobs(Vec<Job*> *jobs);
    void setNodeParams(float *inTrans4x4, float r, float g, float b, float a);
    void begin();
    void end();

private:
    bool pushQuad(Job *job);
    void flushQuads();
    Job *mPrevQuadJob;

    Vec<Job*> *mJobs;

    static const int cMaxVerts = 65535;
    Vec<float> mXY;
    Vec<float> mUV;
    Vec<int> mC;
    int mI_n;
    VBO *mXYvbo;
    VBO *mUVvbo;
    VBO *mCvbo;
    EBO *mQIebo;
};

extern JobsPool *gNevoJobsPool;
extern NevoRenderPipeline gNevoRender;

}

#endif