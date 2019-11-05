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
		// FBX SDK ������ ��ü ����
		// ��� FBX ���� �۾��� �� �����ڷ� �۾��Ѵ�
		g_pFbxSdkManager = FbxManager::Create();

		// IOSettings ��ü ���� �� ����
		// ���� ��Ҹ� ���Ͽ��� �������ų� ���Ϸ� �������� ���� ����
		// FbxImporter, FbxExporter ��ü�� SDK �����ڰ� ���޵��� �� �ν��Ͻ��� �Ϸ�Ǿ���Ѵ�
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	// FbxImporter ��ü ����
	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");

	// FbxScene ��ü ����
	// FbxScene ��ü�� mesh, light, animation ��� ���� �پ��� ��Ҹ� ������ �� �ִ�
	// �� ��ҵ��� ���� �����ؼ� �����Ǿ���Ѵ� = ���� �������� ����
	// ���� ����� �� ��� ��ҵ� ���� ����Ǹ�
	// ���� �Ҹ��� �� ��� ��ҵ� ���� �Ҹ�ȴ�
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	// Importer�ʱ�ȭ�� ���� ���������� fbx ������ ��������
	bool bSuccess = pImporter->Initialize("../Engine/data/FBX/Wolf/Wolf.fbx", -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return false;

	// fbx ���� ������ scene���� �����´�.
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return false;

	// �����ʹ� �� ������ �Ҹ��Ų��
	pImporter->Destroy();

	// ���� ���� ��Ʈ������ ��ȸ�ϸ鼭 ���ϴ� ������ �����´�
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			// ������ ����� �ڽĿ��� �޽� Ÿ�Ը� �Ÿ���
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			// �޽��� ������ �����´�
			FbxVector4* pVertices = pMesh->GetControlPoints();

			// �޽��� ������ ����ŭ �ݺ��Ѵ�
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				// �������� ��� ������ �̷���� �ִ��� �����´�
				int iNumVertices = pMesh->GetPolygonSize(j);
				//if (iNumVertices == 3)
					//return false;
				//assert(iNumVertices == 3);

				// �������� ���ؽ��� �����Ͽ� ���ڷ� ���� ������ ���Ϳ� �־��ش�
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
