#include "CJson.h"

CJSonDoc CJson::parse(QByteArray data)
{
	CJSonDoc ret(this);
	ParsingState state = ParsingStateWaitingObject;
	QString key, value;
	int line = 1;
	bool escape = false;

	key.reserve(1024);
	value.reserve(1024);

	for (int i = 0; i < data.length(); i++)
	{
		qDebug(tr("state: %1 sym: %2").arg(
				   QString::number((int)state),
				   QString(data.at(i))).toLatin1().constData());
		
		if (data.at(i) == '\n')
			line++;

		// Skip eskape symbols
		if (strchr(CJSON_ESCAPE_SYMBOLS, data.at(i)))
		{
			continue;
		}

		switch (data.at(i))
		{
			// Begin of JSON object.
			case CJSON_RESERVED_BEGINOBJECT:
				switch (state)
				{
					// Waiting '{' symbol at begin
					case ParsingStateWaitingObject:
						state = ParsingStateWaitingKey;
						break;

					// Waiting key name starting with '"'
					case ParsingStateWaitingKey:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;
						
					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						break;

					// Value parsing process currently
					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
						{
							
						}						
						break;
					
					// Value parsed now waiting for list separator ','
					case ParsingWaitingList:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
				}
				break;

			// End of JSON object.
			case CJSON_RESERVED_ENDOBJECT:
				switch (state)
				{
					case ParsingStateWaitingObject:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					case ParsingStateWaitingKey:
						state = ParsingStateWaitingObject;
						break;

					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;

					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					case ParsingStateWaitingValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
						{
							state = ParsingWaitingList;
						}
						break;
						
					case ParsingWaitingList:
						break;						
				}
				break;
			
			// Begin of JSON array.
			case CJSON_RESERVED_BEGINARRAY:
				switch (state)
				{
					// Waiting '{' symbol at begin
					case ParsingStateWaitingObject:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Waiting key name starting with '"'
					case ParsingStateWaitingKey:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;
						
						// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						value += data.at(i);
						state = ParsingStateValue;
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						value += data.at(i);
						break;
						
						// Value parsed now waiting for list separator ','
					case ParsingWaitingList:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;

			// End of JSON array.
			case CJSON_RESERVED_ENDARRAY:
				switch (state)
				{
					// Waiting '{' symbol at begin
					case ParsingStateWaitingObject:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Waiting key name starting with '"'
					case ParsingStateWaitingKey:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;
						
						// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
						// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						value += data.at(i);
						state = ParsingWaitingList;
						break;
						
						// Value parsed now waiting for list separator ','
					case ParsingWaitingList:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;
				
			// Begin or end of string (if not escaped).
			case CJSON_RESERVED_STRING:
				switch (state)
				{
					case ParsingStateWaitingObject:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					case ParsingStateKey:
						if (escape)
						{
							key += data.at(i);
						}
						else
							state = ParsingWaitingSeparatorValue;
						break;

					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					case ParsingStateWaitingKey:
						state = ParsingStateKey;
						break;

					case ParsingStateWaitingValue:
						state = ParsingStateValue;
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
							state = ParsingStateWaitingKey;
						break;
						
					case ParsingWaitingList:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;
				
			case CJSON_RESERVED_VALUE:
				switch (state)
				{
					// Waiting '{' symbol at begin
					case ParsingStateWaitingObject:
						state = ParsingStateWaitingKey;
						break;

					// Waiting key name starting with '"'
					case ParsingStateWaitingKey:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;
						
					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						state = ParsingStateWaitingValue;
						break;


					// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

					// Value parsing process currently
					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
						{
							emitParsingError(data.at(i), line);
							return CJSonDoc();
						}
						break;
					
					// Value parsed now waiting for list separator ','
					case ParsingWaitingList:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
				}
				break;

		}

		escape = data.at(i) == CJSON_RESERVED_ESCAPE;
	}
	
	return ret;
}

CJSonDoc CJson::parse(QString data)
{
	return parse(data.toLatin1());
}

CJson::CJson(QObject *parent) :
	QObject(parent)
{
}

void CJson::emitParsingError(char symbol, int line)
{
	qDebug(tr("Unexpected symbol %1 at line %2").arg(
			   QString(symbol),
			   QString::number(line)).toLatin1().constData());
	
	emit parsingFailed(-1, tr("Unexpected symbol %1 at line %2").arg(
						   QString(symbol),
						   QString::number(line)));
}
