#ifndef _FBXIMPORTER_H_
#define _FBXIMPORTER_H_


#include <fbxsdk.h>
#include <vector>

class FBXImporter
{
public:
	FBXImporter();
	FBXImporter(const FBXImporter&);
	~FBXImporter();

	struct MyVertex
	{
		float pos[3];
	};

	bool LoadFBX(std::vector<MyVertex>* pOutVertexVector);

	FbxManager* g_pFbxSdkManager = nullptr;
};

#endif // !_FBXIMPORTER_H_
