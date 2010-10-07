#ifndef LYTBOUNDING_H
#define LYTBOUNDING_H

#include "common.h"
#include "pane.h"

// the most useful object in LYT
// it's just a regular Pane ... with a different name.

class LYTBounding : public LYTPane {
public:
	LYTBounding(LYTLayout &layout);


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true);
};


#endif // LYTPICTURE_H
