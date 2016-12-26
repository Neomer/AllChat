#include "CJson.h"

CJSonDoc CJson::parse(QByteArray data)
{
	CJSonDoc ret(this);
	ParsingState state = ParsingStateWaitingObject;
	QString key, value;
	CJSonElement::JsonElementType type;
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
//		if (strchr(CJSON_ESCAPE_SYMBOLS, data.at(i)))
//		{
//			continue;
//		}

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
						state = ParsingStateValue;

						// TODO recursive! <<=						
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
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
				}
				break;

			// End of JSON object.
			case CJSON_RESERVED_ENDOBJECT:
				switch (state)
				{
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

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							key.clear();
							value.clear();
						}
						break;
						
					case ParsingWaitingList:
						// Last element
						break;						
					
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();

				}
				break;
			
			// Begin of JSON array.
			case CJSON_RESERVED_BEGINARRAY:
				switch (state)
				{
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
						
						// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						//value += data.at(i);
						type = CJSonElement::JsonTypeArray;
						state = ParsingStateValue;
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						value += data.at(i);
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;

			// End of JSON array.
			case CJSON_RESERVED_ENDARRAY:
				switch (state)
				{
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
						
						// Value parsing process currently
					case ParsingStateValue:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							value += data.at(i);
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							key.clear();
							value.clear();
						}
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;
				
			// Begin or end of string (if not escaped).
			case CJSON_RESERVED_STRING:
				switch (state)
				{
					case ParsingStateKey:
						if (escape)
						{
							key += data.at(i);
						}
						else
							state = ParsingWaitingSeparatorValue;
						break;

					case ParsingStateWaitingKey:
						state = ParsingStateKey;
						break;

					case ParsingStateWaitingValue:
						type = CJSonElement::JsonTypeString;
						state = ParsingStateValue;
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(i);
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							key.clear();
							value.clear();
						}
						break;
						
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}
				break;
			
			// Symbol ':'
			case CJSON_RESERVED_VALUE:
				switch (state)
				{
					// Waiting '{' symbol at begin
					case ParsingStateWaitingObject:
						state = ParsingStateWaitingKey;
						break;

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
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
						
				}
				break;
				
			// Symbol ','
			case CJSON_RESERVED_LIST:
				switch (state)
				{
					case ParsingStateKey:
						key += CJSON_RESERVED_LIST;
						break;
					
					case ParsingStateValue:
						value += CJSON_RESERVED_LIST;
						break;
						
					case ParsingStateWaitingKey:
						// skip symbol
						break;
						
					case ParsingWaitingList:
						state = ParsingStateWaitingKey;
						break;
						
					default:
						emitParsingError(data.at(i), line);
						return CJSonDoc();
				}

				break;
			
			// Space
			case ' ':
				switch (state)
				{
					case ParsingStateKey:
						key += data.at(i);
						break;
						
					case ParsingStateValue:
						key += data.at(i);
						break;
						
					default:
						break;
				}

				break;
				
			default:
				switch (state)
				{
					// Key name parsing currently
					case ParsingStateKey:
						key += data.at(i);
						break;
						
					// Value parsing process currently
					case ParsingStateValue:
						value += data.at(i);
						break;
					
					// Value parsed now waiting for list separator ','
					default:
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
