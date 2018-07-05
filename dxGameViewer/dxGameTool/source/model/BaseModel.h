//====================================================================================
//		## BaseModel ## (모델의 기본정보를 모아둔 클래스)
//		## 모델의 정점과 인덱스 버퍼는 row ptr을 사용하므로 삭제시 꼭 해제해야 한다.
//====================================================================================
#pragma once
#include "..\shader\Shader.h"
#include "ModelStructure.h"


class BaseModel
{
public:
	BaseModel();
	virtual ~BaseModel() {};

	virtual void				Release();
	virtual void				Render(ID3D11DeviceContext* dc)	PURE;
		

	bool						CreateModel(ID3D11Device* device, Vertex& vertices, vector<unsigned long>& indices, 
										D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	template <typename T>
	bool						CreateBuffer(ID3D11Device* device, ID3D11Buffer** buffer, vector<T>& data, bool dynamicBuffer = false);

	bool						InitVertextBuffers(ID3D11Device*, Vertex&, bool dynamicBuffer = false);
	bool						InitIndexBuffers(ID3D11Device*, vector<unsigned long>&);
	
	XMMATRIX					UpdateModelMatrix();
	
	
	//Set Function
	void						SetShader(Shader* shader, bool buildLayoutInfo = true);
	void						SetRenderBuffers(ID3D11DeviceContext* dc);

	void						SetCenter(XMFLOAT3 center) {_center = center;}
	void						SetRotate(XMFLOAT3 rotate) { _rotate = rotate; }
	void						SetSize(XMFLOAT3 size) { _size = size; }
	
	void						SetNormalize(bool isNormalize)						{	_isNormalize = isNormalize;		}	
	void						SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type)		{	_primitiveType = type;			}
	void						SetName(wstring name)								{	_name = name;					}
	void						SetScaleFactor(float scaleFactor)					{	_scaleFactor =scaleFactor;		}
	void						SetIndexCnt(int idxCnt)								{	_indexCount = idxCnt;			}

	//Get Function
	Shader*						GetShader()											{	return	_shader;				}
	bool						IsNormalize()										{	return	_isNormalize;			}
	float						GetScaleFactor()									{	return	_scaleFactor;			}
	wstring						GetName()											{	return	_name;					}
	XMFLOAT3					GetSize()											{	return	_size;					}
	XMFLOAT3					GetRotate()											{	return	_rotate;				}
	XMFLOAT3					GetCenter()											{	return	_center;				}
	VertexDataBuffer*			GetVertexBuffer()									{	return	&_vertexDataBuffer;		}
	
//==================================== 멤버 변수 =======================================//

protected:
	//정점 정보가 저장된 데이터 버퍼
	VertexDataBuffer			_vertexDataBuffer;

	//vertexDataBuffer에서 사용될 버퍼만 연결한다.
	vector<ID3D11Buffer**>		_vertexBuffer;							//버텍스버퍼
	vector<unsigned int>		_vertexStride;							//버텍스 버퍼 섹터 크기

	ID3D11Buffer*				_indexBuffer;							//인덱스 버퍼
	wstring						_name;


	XMFLOAT3					_rotate;								//회전각
	XMFLOAT3					_center;								//위치
	XMFLOAT3					_size;									//크기
	XMMATRIX					_tmModel;								//Model TM


	bool						_isNormalize;							//모델 크기 정규화
	float						_scaleFactor;							//정규화에 사용될 크기 값
	UINT						_vertexCount, _indexCount;				//모델의 정점 및 인덱스 수

	D3D11_PRIMITIVE_TOPOLOGY	_primitiveType;							//프리미티브
	Shader*						_shader;								//사용할 셰이더
	
};
