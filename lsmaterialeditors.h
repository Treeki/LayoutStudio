#ifndef LSMATERIALEDITORS_H
#define LSMATERIALEDITORS_H

#include "lsseteditor.h"
#include "lyt/materials/material.h"
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QFormLayout>

class LSTevStageEditor : public LSSetEntryEditorBase<LYTTevStage> {
	Q_OBJECT

	QComboBox *m_texCoord, *m_colour, *m_texMap;
	QCheckBox *m_disableTex;

	QComboBox *m_rasSwapMode, *m_texSwapMode;

	QComboBox *m_colourInA, *m_colourInB, *m_colourInC, *m_colourInD;

	QComboBox *m_colourOp, *m_colourBias, *m_colourScale, *m_colourOutReg;
	QCheckBox *m_colourClamp;
	QComboBox *m_colourConst;

	QComboBox *m_alphaInA, *m_alphaInB, *m_alphaInC, *m_alphaInD;

	QComboBox *m_alphaOp, *m_alphaBias, *m_alphaScale, *m_alphaOutReg;
	QCheckBox *m_alphaClamp;
	QComboBox *m_alphaConst;

	QComboBox *m_indStage, *m_indFormat, *m_indBias, *m_indMatrix;
	QComboBox *m_indWrapS, *m_indWrapT, *m_indAlphaSel;
	QCheckBox *m_indAddPrev, *m_indUtcLod;

public:
	explicit LSTevStageEditor(QWidget *parent = 0) :
		LSSetEntryEditorBase(parent) {

		// Order
		m_texCoord = new QComboBox(this);
		m_texCoord->addItem("No TexCoord");
		for (int i = 0; i < 8; i++)
			m_texCoord->addItem(QString("TexCoord %1").arg(i));

		m_colour = new QComboBox(this);
		m_colour->addItem("Ignore Rasterised Colour");
		m_colour->addItem("Colour Channel 0");
		m_colour->addItem("Colour Channel 1");
		m_colour->addItem("Zero Colour");
		m_colour->addItem("Alpha Bump(?)");
		m_colour->addItem("Alpha BumpN(?)");

		m_texMap = new QComboBox(this);
		m_texMap->addItem("No TexMap");
		for (int i = 0; i < 8; i++)
			m_texMap->addItem(QString("TexMap %1").arg(i));

		m_disableTex = new QCheckBox("Disable Texture Lookup", this);

		// SwapModes
		m_rasSwapMode = new QComboBox(this);
		m_texSwapMode = new QComboBox(this);
		for (int i = 0; i < 4; i++) {
			m_rasSwapMode->addItem(QString("Swap Mode Table %1").arg(i));
			m_texSwapMode->addItem(QString("Swap Mode Table %1").arg(i));
		}

		// Colour in
		static const char *colIn[] = {
			"Colour Reg0", "Colour Reg1", "Colour Reg2", "Colour Reg3",
			"Alpha Reg0", "Alpha Reg1", "Alpha Reg2", "Alpha Reg3",
			"Texture Colour", "Texture Alpha",
			"Rasterised Colour", "Rasterised Alpha",
			"1.0", "0.5", "0.0", "Constant Colour"
		};

		m_colourInA = new QComboBox(this);
		m_colourInB = new QComboBox(this);
		m_colourInC = new QComboBox(this);
		m_colourInD = new QComboBox(this);

		for (int i = 0; i < 16; i++) {
			m_colourInA->addItem(colIn[i]);
			m_colourInB->addItem(colIn[i]);
			m_colourInC->addItem(colIn[i]);
			m_colourInD->addItem(colIn[i]);
		}

		// Colour op
		m_colourOp = new QComboBox(this);
		m_colourOp->addItem("Add: (d + ((1.0 - c)*a + c*b) + bias) * scale");
		m_colourOp->addItem("Sub: (d - ((1.0 - c)*a + c*b) + bias) * scale");
		m_colourOp->addItem("Comp_R8_GT: d + ((a[R] > b[R]) ? c : 0)");
		m_colourOp->addItem("Comp_R8_EQ: d + ((a[R] == b[R]) ? c : 0)");
		m_colourOp->addItem("Comp_GR16_GT: d + ((a[GR] > b[GR]) ? c : 0)");
		m_colourOp->addItem("Comp_GR16_EQ: d + ((a[GR] == b[GR]) ? c : 0)");
		m_colourOp->addItem("Comp_BGR24_GT: d + ((a[BGR] > b[BGR]) ? c : 0)");
		m_colourOp->addItem("Comp_BGR24_EQ: d + ((a[BGR] == b[BGR]) ? c : 0)");
		m_colourOp->addItem("Comp_RGB8_GT: Some crap");
		m_colourOp->addItem("Comp_RGB8_EQ: Some crap");

		m_colourBias = new QComboBox(this);
		m_colourBias->addItem("Zero");
		m_colourBias->addItem("+0.5");
		m_colourBias->addItem("-0.5");

		m_colourScale = new QComboBox(this);
		m_colourScale->addItem("x1");
		m_colourScale->addItem("x2");
		m_colourScale->addItem("x4");
		m_colourScale->addItem("x0.5");

		m_colourClamp = new QCheckBox("Clamp between 0.0 and 1.0", this);

		m_colourOutReg = new QComboBox(this);
		m_colourOutReg->addItem("Register 0");
		m_colourOutReg->addItem("Register 1");
		m_colourOutReg->addItem("Register 2");
		m_colourOutReg->addItem("Register 3 (Final output)");

		static const char *colConst[] = {
			"Constant 0", "Constant 1", "Constant 2", "Constant 3",
			"Const 0 Red", "Const 1 Red", "Const 2 Red", "Const 3 Red",
			"Const 0 Green", "Const 1 Green", "Const 2 Green", "Const 3 Green",
			"Const 0 Blue", "Const 1 Blue", "Const 2 Blue", "Const 3 Blue",
			"Const 0 Alpha", "Const 1 Alpha", "Const 2 Alpha", "Const 3 Alpha",
			"1.0", "0.875", "0.75", "0.625",
			"0.5", "0.375", "0.25", "0.125"
		};
		m_colourConst = new QComboBox(this);
		for (int i = 0; i < 28; i++)
			m_colourConst->addItem(colConst[i]);

		// Alpha in
		static const char *alphIn[] = {
			"Alpha Reg0", "Alpha Reg1", "Alpha Reg2", "Alpha Reg3",
			"Texture Alpha", "Rasterised Alpha",
			"0.0", "Constant alpha"
		};

		m_alphaInA = new QComboBox(this);
		m_alphaInB = new QComboBox(this);
		m_alphaInC = new QComboBox(this);
		m_alphaInD = new QComboBox(this);

		for (int i = 0; i < 8; i++) {
			m_alphaInA->addItem(alphIn[i]);
			m_alphaInB->addItem(alphIn[i]);
			m_alphaInC->addItem(alphIn[i]);
			m_alphaInD->addItem(alphIn[i]);
		}

		// alpha op
		m_alphaOp = new QComboBox(this);
		m_alphaOp->addItem("Add: (d + ((1.0 - c)*a + c*b) + bias) * scale");
		m_alphaOp->addItem("Sub: (d - ((1.0 - c)*a + c*b) + bias) * scale");
		m_alphaOp->addItem("Comp_R8_GT: d + ((a[R] > b[R]) ? c : 0)");
		m_alphaOp->addItem("Comp_R8_EQ: d + ((a[R] == b[R]) ? c : 0)");
		m_alphaOp->addItem("Comp_GR16_GT: d + ((a[GR] > b[GR]) ? c : 0)");
		m_alphaOp->addItem("Comp_GR16_EQ: d + ((a[GR] == b[GR]) ? c : 0)");
		m_alphaOp->addItem("Comp_BGR24_GT: d + ((a[BGR] > b[BGR]) ? c : 0)");
		m_alphaOp->addItem("Comp_BGR24_EQ: d + ((a[BGR] == b[BGR]) ? c : 0)");
		m_alphaOp->addItem("Comp_RGB8_GT: Some crap");
		m_alphaOp->addItem("Comp_RGB8_EQ: Some crap");

		m_alphaBias = new QComboBox(this);
		m_alphaBias->addItem("Zero");
		m_alphaBias->addItem("+0.5");
		m_alphaBias->addItem("-0.5");

		m_alphaScale = new QComboBox(this);
		m_alphaScale->addItem("x1");
		m_alphaScale->addItem("x2");
		m_alphaScale->addItem("x4");
		m_alphaScale->addItem("x0.5");

		m_alphaClamp = new QCheckBox("Clamp between 0.0 and 1.0", this);

		m_alphaOutReg = new QComboBox(this);
		m_alphaOutReg->addItem("Register 0");
		m_alphaOutReg->addItem("Register 1");
		m_alphaOutReg->addItem("Register 2");
		m_alphaOutReg->addItem("Register 3 (Final output)");

		static const char *alphConst[] = {
			"Const 0 Red", "Const 1 Red", "Const 2 Red", "Const 3 Red",
			"Const 0 Green", "Const 1 Green", "Const 2 Green", "Const 3 Green",
			"Const 0 Blue", "Const 1 Blue", "Const 2 Blue", "Const 3 Blue",
			"Const 0 Alpha", "Const 1 Alpha", "Const 2 Alpha", "Const 3 Alpha",
			"1.0", "0.875", "0.75", "0.625",
			"0.5", "0.375", "0.25", "0.125"
		};
		m_alphaConst = new QComboBox(this);
		for (int i = 0; i < 24; i++)
			m_alphaConst->addItem(alphConst[i]);


		// Indirect crap
		m_indStage = new QComboBox(this);
		for (int i = 0; i < 4; i++)
			m_indStage->addItem(QString("Stage %1").arg(i));

		m_indFormat = new QComboBox(this);
		m_indFormat->addItem("8 bits");
		m_indFormat->addItem("5 bits");
		m_indFormat->addItem("4 bits");
		m_indFormat->addItem("3 bits");

		m_indBias = new QComboBox(this);
		m_indBias->addItem("No bias");
		m_indBias->addItem("Bias S");
		m_indBias->addItem("Bias T");
		m_indBias->addItem("Bias S, T");
		m_indBias->addItem("Bias U");
		m_indBias->addItem("Bias S, U");
		m_indBias->addItem("Bias T, U");
		m_indBias->addItem("Bias S, T, U");

		m_indMatrix = new QComboBox(this);
		m_indMatrix->addItem("No matrix");
		m_indMatrix->addItem("Matrix 0");
		m_indMatrix->addItem("Matrix 1");
		m_indMatrix->addItem("Matrix 2");
		m_indMatrix->addItem("Matrix S0");
		m_indMatrix->addItem("Matrix S1");
		m_indMatrix->addItem("Matrix S2");
		m_indMatrix->addItem("Matrix T0");
		m_indMatrix->addItem("Matrix T1");
		m_indMatrix->addItem("Matrix T2");

		static const char *IndWraps[] = {
			"No wrap",
			"Wrap 256", "Wrap 128", "Wrap 64", "Wrap 32", "Wrap 16",
			"Zero"
		};
		m_indWrapS = new QComboBox(this);
		m_indWrapT = new QComboBox(this);

		for (int i = 0; i < 7; i++) {
			m_indWrapS->addItem(IndWraps[i]);
			m_indWrapT->addItem(IndWraps[i]);
		}

		m_indAddPrev = new QCheckBox("Add previous", this);
		m_indUtcLod = new QCheckBox("UTC LOD", this);

		m_indAlphaSel = new QComboBox(this);
		m_indAlphaSel->addItem("No bump alpha");
		m_indAlphaSel->addItem("From S");
		m_indAlphaSel->addItem("From T");
		m_indAlphaSel->addItem("From U");

		// Now put together the whole thing
		QFormLayout *layout = new QFormLayout(this);

		layout->addRow("Texture Map", m_texMap);
		layout->addWidget(m_disableTex);
		layout->addRow("Texture Coordinates", m_texCoord);
		layout->addRow("Rasterised Colour Channel", m_colour);

		layout->addRow("Texture Swap Mode Table", m_texSwapMode);
		layout->addRow("Rast. Swap Mode Table", m_rasSwapMode);

		layout->addWidget(new QLabel("<b>Colour Computation:</b>", this));
		layout->addRow("Input A", m_colourInA);
		layout->addRow("Input B", m_colourInB);
		layout->addRow("Input C", m_colourInC);
		layout->addRow("Input D", m_colourInD);
		layout->addRow("Constant", m_colourConst);

		layout->addRow("Equation", m_colourOp);
		layout->addRow("Bias", m_colourBias);
		layout->addRow("Scale", m_colourScale);
		layout->addWidget(m_colourClamp);
		layout->addRow("Output Register", m_colourOutReg);

		layout->addWidget(new QLabel("<b>Alpha Computation:</b>", this));
		layout->addRow("Input A", m_alphaInA);
		layout->addRow("Input B", m_alphaInB);
		layout->addRow("Input C", m_alphaInC);
		layout->addRow("Input D", m_alphaInD);
		layout->addRow("Constant", m_alphaConst);

		layout->addRow("Equation", m_alphaOp);
		layout->addRow("Bias", m_alphaBias);
		layout->addRow("Scale", m_alphaScale);
		layout->addWidget(m_alphaClamp);
		layout->addRow("Output Register", m_alphaOutReg);

		layout->addWidget(new QLabel("<b>Indirect Textures:</b>", this));
		layout->addRow("Indirect Stage", m_indStage);
		layout->addRow("Format", m_indFormat);
		layout->addRow("Bias", m_indBias);
		layout->addRow("Matrix", m_indMatrix);
		layout->addRow("Wrap S", m_indWrapS);
		layout->addRow("Wrap S", m_indWrapT);
		layout->addWidget(m_indAddPrev);
		layout->addWidget(m_indUtcLod);
		layout->addRow("Alpha Sel", m_indAlphaSel);

	}

	void loadEntryFrom(const LYTTevStage &entry) {
		m_texCoord->setCurrentIndex(entry.texCoord == 0xFF ? 0 : (entry.texCoord + 1));

		// TEV order
		quint8 actualTexMap = (entry.texMap & 0xFF);
		bool disableTexMap = (entry.texMap & 0x100) != 0;
		m_texMap->setCurrentIndex(actualTexMap == 0xFF ? 0 : (actualTexMap + 1));
		m_disableTex->setChecked(disableTexMap);

		static const int ColourIndices[] = {
			-1, -1, -1, -1, 1, 2, 3, 4, 5
		};

		if (entry.colour == 0xFF)
			m_colour->setCurrentIndex(0);
		else if (entry.colour > 8)
			m_colour->setCurrentIndex(-1);
		else
			m_colour->setCurrentIndex(ColourIndices[entry.colour]);


		// swapmode
		m_rasSwapMode->setCurrentIndex(entry.rasSwapMode);
		m_texSwapMode->setCurrentIndex(entry.texSwapMode);

		// colour in
		static const int ColInIndices[] = {
			3, 7, 0, 4, 1, 5, 2, 6,
			8, 9, 10, 11, 12, 13, 14, 15
		};
		m_colourInA->setCurrentIndex(ColInIndices[entry.colourInA]);
		m_colourInB->setCurrentIndex(ColInIndices[entry.colourInB]);
		m_colourInC->setCurrentIndex(ColInIndices[entry.colourInC]);
		m_colourInD->setCurrentIndex(ColInIndices[entry.colourInD]);

		// colour op
		static const int OpIndices[] = {
			0, 1, -1, -1, -1, -1, -1, -1,
			2, 3, 4, 5, 6, 7, 8, 9
		};
		m_colourOp->setCurrentIndex(OpIndices[entry.colourOp]);

		m_colourBias->setCurrentIndex(entry.colourBias);
		m_colourScale->setCurrentIndex(entry.colourScale);
		m_colourClamp->setChecked(entry.colourClamp);

		static const int RegIndices[] = {
			3, 0, 1, 2
		};
		m_colourOutReg->setCurrentIndex(RegIndices[entry.colourOutReg]);

		static const int KCSelIndices[] = {
			20, 21, 22, 23, 24, 25, 26, 27,
			-1, -1, -1, -1,
			0, 1, 2, 3,
			4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
		};
		m_colourConst->setCurrentIndex(KCSelIndices[entry.colourConst]);

		// alpha in
		static const int AlphInIndices[] = {
			3, 0, 1, 2, 4, 5, 6, 7,
			-1, -1, -1, -1, -1, -1, -1, -1
		};
		m_alphaInA->setCurrentIndex(AlphInIndices[entry.alphaInA]);
		m_alphaInB->setCurrentIndex(AlphInIndices[entry.alphaInB]);
		m_alphaInC->setCurrentIndex(AlphInIndices[entry.alphaInC]);
		m_alphaInD->setCurrentIndex(AlphInIndices[entry.alphaInD]);

		// alpha op
		m_alphaOp->setCurrentIndex(OpIndices[entry.alphaOp]);

		m_alphaBias->setCurrentIndex(entry.alphaBias);
		m_alphaScale->setCurrentIndex(entry.alphaScale);
		m_alphaClamp->setChecked(entry.alphaClamp);

		m_alphaOutReg->setCurrentIndex(RegIndices[entry.alphaOutReg]);

		static const int KASelIndices[] = {
			16, 17, 18, 19, 20, 21, 22, 23,
			-1, -1, -1, -1, -1, -1, -1, -1,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
		};
		m_alphaConst->setCurrentIndex(KCSelIndices[entry.alphaConst]);


		// indirect crap
		static const int IndMtxIndices[] = {
			0, 1, 2, 3, -1, 4, 5, 6, -1, 7, 8, 9, -1, -1, -1, -1
		};

		m_indStage->setCurrentIndex(entry.indStage);
		m_indFormat->setCurrentIndex(entry.indFormat);
		m_indBias->setCurrentIndex(entry.indBias);
		m_indMatrix->setCurrentIndex(IndMtxIndices[entry.indMatrix]);
		m_indWrapS->setCurrentIndex(entry.indWrapS);
		m_indWrapT->setCurrentIndex(entry.indWrapT);
		m_indAddPrev->setChecked(entry.indAddPrev);
		m_indUtcLod->setChecked(entry.indUtcLod);
		m_indAlphaSel->setCurrentIndex(entry.indAlphaSel);
	}
};


#endif // LSMATERIALEDITORS_H
