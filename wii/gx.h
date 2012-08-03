#ifndef WII_GX_H
#define WII_GX_H

namespace GX {
	enum TextureFormat {
		I4 = 0,
		I8 = 1,
		IA4 = 2,
		IA8 = 3,
		RGB565 = 4,
		RGB5A3 = 5,
		RGBA8 = 6,
		CI4 = 8,
		CI8 = 9,
		CI14X2 = 10,
		CMPR = 14
	};

	enum WrapType {
		Clamp = 0,
		Repeat = 1,
		Mirror = 2
	};

	enum TextureFilter {
		Near = 0,
		Linear = 1,
		NearMipNear = 2,
		LinMipNear = 3,
		NearMipLin = 4,
		LinMipLin = 5
	};
};

#endif // WII_GX_H
