#include "FBXImporter.h"


FBXImporter::FBXImporter()
{
}

FBXImporter::FBXImporter(const FBXImporter &)
{
}

FBXImporter::~FBXImporter()
{
}

bool FBXImporter::LoadFBX(std::vector<MyVertex>* pOutVertexVector)
{
	if (g_pFbxSdkManager == nullptr)
	{
		// FBX SDK 관리자 객체 생성
		// 모든 FBX 관련 작업은 이 관리자로 작업한다
		g_pFbxSdkManager = FbxManager::Create();

		// IOSettings 객체 생성 및 설정
		// 씬의 요소를 파일에서 가져오거나 파일로 내보낼지 여부 지정
		// FbxImporter, FbxExporter 객체에 SDK 관리자가 전달되지 전 인스턴스가 완료되어야한다
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	// FbxImporter 객체 생성
	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");

	// FbxScene 객체 생성
	// FbxScene 객체는 mesh, light, animation 등과 같은 다양한 요소를 포함할 수 있다
	// 이 요소들은 씬을 참조해서 생성되어야한다 = 씬에 종속적인 관계
	// 씬이 추출될 때 모든 요소도 같이 추출되며
	// 씬이 소멸할 때 모든 요소도 같이 소멸된다
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	// Importer초기화를 통해 가져오려는 fbx 파일을 가져오고
	bool bSuccess = pImporter->Initialize("../Engine/data/FBX/Wolf/Wolf.fbx", -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return false;

	// fbx 파일 내용을 scene으로 가져온다.
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return false;

	// 임포터는 다 썻으니 소멸시킨다
	pImporter->Destroy();

	// 이제 씬의 루트노드부터 순회하면서 원하는 정보를 가져온다
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			// 가져온 노드의 자식에서 메쉬 타입만 거른다
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			// 메쉬의 정점을 가져온다
			FbxVector4* pVertices = pMesh->GetControlPoints();

			// 메쉬의 폴리곤 수만큼 반복한다
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				// 폴리곤이 몇개의 점으로 이루어져 있는지 가져온다
				int iNumVertices = pMesh->GetPolygonSize(j);
				//if (iNumVertices == 3)
					//return false;
				//assert(iNumVertices == 3);

				// 폴리곤의 버텍스를 저장하여 인자로 받은 포인터 벡터에 넣어준다
				for (int k = 0; k < iNumVertices; k++) {
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					MyVertex vertex;
					vertex.pos[0] = (float)pVertices[iControlPointIndex].mData[0];
					vertex.pos[1] = (float)pVertices[iControlPointIndex].mData[1];
					vertex.pos[2] = (float)pVertices[iControlPointIndex].mData[2];
					pOutVertexVector->push_back(vertex);
				}
			}

		}

	}
	return true;
}
