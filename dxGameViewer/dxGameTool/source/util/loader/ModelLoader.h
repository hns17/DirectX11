//==================================================================
//		## ModelLoader ## (assimp 라이브러리를 이용해 모델 정보를 로드한다.)
//==================================================================


#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>


class ModelLoader
{
public:
	ModelLoader() {};
	~ModelLoader() {};

	//모델 정보 로드
	SkinModel*	LoadModel(CString path, UINT flag);
	void		LoadAnimation(CString path, SkinModel* model, UINT flag);
protected:
	//노드 정보 생성(본 정보)
	void ProcessNode(aiNode* aiNodeInfo, SkinModel* skModel, NodeInfo* parent = NULL, int depth = 0);
	//메쉬 정보 생성(정점정보, 인덱스 정보, 기본 메쉬 정보)
	void ProcessMesh(aiMesh* mesh, Vertex& vertices, vector<unsigned long>& indices, vector<HierarchyMesh*>& meshList);
	//재질 정보 생성
	void ProcessMaterial(const aiScene* pScene, vector<Material>& matList, CString directoryPath);
	//스키닝 정보 생성(offset_mat, weight)
	void ProcessSkin(aiMesh* aiMesh, HierarchyMesh* mesh, Vertex& vertices, vector<unsigned long>& indices, SkinModel* skModel);
	//애니메이션 정보 생성
	void ProcessAnimation(const aiScene* pScene, SkinModel* skModel);

	//이름을 통해 노드를 찾는다.
	NodeInfo* FindNode(aiString name, vector<NodeInfo*> nodeList);
};
