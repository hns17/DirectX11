#pragma once
#include "BaseModel.h"

class ColorModel : public BaseModel
{
public:
	ColorModel();
	~ColorModel() {};

	void Render(ID3D11DeviceContext* dc);
	void SetColor(XMFLOAT4 color);
	void TurnOnColor();
	void TurnOffColor();

private:
	XMFLOAT4 _color;
	bool	 _useColor = true;
};
