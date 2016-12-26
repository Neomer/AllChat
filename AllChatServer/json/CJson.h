#ifndef CJSON_H
#define CJSON_H

#include <QObject>
#include <QByteArray>
#include "CJsonDoc.h"

#define CJSON_ESCAPE_SYMBOLS				"\r\n\t"

// Allow escaped symblos in key names
#ifndef CJSON_DISABLE_ESCAPEDKEY
#define CJSON_SETTINGS_ALLOWESCAPEDKEY		false
#endif

#define CJSON_RESERVED_BEGINOBJECT		'{'
#define CJSON_RESERVED_ENDOBJECT		'}'
#define CJSON_RESERVED_BEGINARRAY		'['
#define CJSON_RESERVED_ENDARRAY			']'
#define CJSON_RESERVED_ESCAPE			'\\'
#define CJSON_RESERVED_STRING			'"'
#define CJSON_RESERVED_LIST				','
#define CJSON_RESERVED_VALUE			':'


class CJson : public QObject
{
	Q_OBJECT

public:
	static CJson instance(QObject * parent = 0) { static CJson __inst(parent); return __inst; }

	CJsonDoc parse(QByteArray data, int * start);
	CJsonDoc parse(QString data);

signals:
	void parsingFailed(int errorNum, QString errorDescription);

private:
	CJson(QObject * parent = 0);
	CJson(const CJson& root) { };
	CJson& operator=(const CJson&);

	void emitParsingError(char symbol, int line);

	///
	/// \brief The ParsingState enum State of parsing process
	///
	enum ParsingState
	{
		ParsingStateWaitingObject,			// 0
		ParsingStateWaitingKey,				// 1
		ParsingStateKey,					// 2
		ParsingStateWaitingValue,			// 3
		ParsingStateValue,					// 4
		ParsingWaitingList,					// 5
		ParsingWaitingSeparatorValue,		// 6
	};
};

#endif // CJSON_H
