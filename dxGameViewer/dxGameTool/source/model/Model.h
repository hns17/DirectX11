#pragma once

//Vertex Info, 나중에 normal 정보 추가
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	
	XMFLOAT4 tangent;
	XMFLOAT3 bitangent;
};

struct Material
{
	wstring name;
	
	Texture* diffuseMap;
	Texture* alphaMap;
	Texture* normalMap;
	Texture* sepcularMap;
	Texture* lightMap;

	Material() {
		diffuseMap	= NULL;
		alphaMap	= NULL;
		normalMap	= NULL;
		sepcularMap = NULL;
		lightMap	= NULL;
	}
};

struct Mesh {
	wstring	name;	//Object Name Info
	
	int start;		//Object Start Idx or Vert Info
	int count;		//Object Idx or Vert Count Info
	
	bool isDraw;

	Material* mat;

	Mesh() {
		name = L"";
		isDraw = true;
		start	= -1;
		count	= -1;
		
		mat = NULL;
	}

	Mesh(wstring strName, int iStart, int iCount) {
		Mesh();
		name = strName;
		start = iStart;
		count = iCount;

		mat = NULL;
	}
	
};

struct Node {
	Node* parrentNode;
	vector<Node*> childNode;
	Mesh* nodeData;

	Node(Mesh* meshData = NULL) {
		parrentNode = NULL;
		nodeData = meshData;
	}

	~Node() {
		SAFE_DELETE(nodeData);
		
		vector<Node*>::iterator iter = childNode.begin();
		for (iter; iter != childNode.end(); iter++) {
			SAFE_DELETE(*iter);
		}

		vector<Node*>().swap(childNode);
	}
};

class Model
{
public:
	Model();
	virtual ~Model();

	//함수 이름이 곧 설명...
	bool CreateModel(ID3D11Device*, vector<Vertex>, vector<unsigned long>);
	void InsertNode(Node* node);
	void InsertMaterial(Material* mat);
	void Release();

	virtual void Render(ID3D11DeviceContext*, PWindowRender pWR);
	

	void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type);
	void SetCenter(XMFLOAT3 center);
	void SetRotate(XMFLOAT3 rotate);
	void SetSize(XMFLOAT3 size);
	void SetRotVelocity(XMFLOAT3 rot);		
	void SetShader(wstring path);
	void SetName(wstring name);
	void SetScaleFactor(float scaleFactor);

	void		SetNormalize(bool isNormalize);
	bool		IsNormalize();


	int			GetIndexCount();
	int			GetVertexCount();
	float		GetScaleFactor();
	wstring		GetName();

	XMFLOAT3	GetSize();
	XMFLOAT3	GetRotate();
	XMFLOAT3	GetCenter();
	XMFLOAT3	GetRotVelocity();
	
	vector<Node*>&	GetNodeList();
	vector<Material*>&	GetMaterialList();

protected:
	bool InitVertextBuffers(ID3D11Device*, vector<Vertex>);
	bool InitIndexBuffers(ID3D11Device*, vector<unsigned long>);
	void RenderBuffers(ID3D11DeviceContext*);
	void RenderNode(ID3D11DeviceContext* deviceContext, PWindowRender pWR, Node* node);

protected:
	wstring	_name;										//모델 명
	bool	_isNormalize;								//모델 크기 정규화
	int		_vertexCount, _indexCount;					//모델의 정점 및 인덱스 수
	float	_scaleFactor;								//정규화에 사용될 크기 값

	ID3D11Buffer *_vertexBuffer, *_indexBuffer;			//버퍼
	D3D11_PRIMITIVE_TOPOLOGY	_primitiveType;			//프리미티브

	XMFLOAT3					_rotate;				//회전각
	XMFLOAT3					_center;				//위치
	XMFLOAT3					_size;					//크기
	XMFLOAT3					_rotVelocity;			//모델 회전 속도
	XMVECTOR					_accVel;				//회전속도에 따른 가속도
	
	vector<Material*>			_matList;
	vector<Node*>				_nodeList;				//Obj List, 모델은 여러 오브젝트로 구성됨

	Shader*						_shader;				//사용할 셰이더
};