#include "CJson.h"

CJsonDoc CJson::parse(QByteArray data, int *start)
{
	CJsonDoc ret(this);
	ParsingState state = ParsingStateWaitingObject;
	QString key, value;
	CJsonElement::JsonElementType type;
	int line = 1;
	bool escape = false;

	qDebug("Start parsing %s from %d",
		   data.constData(), *start);

	key.reserve(1024);
	value.reserve(1024);

	while (*start < data.length())
	{
		qDebug(tr("state: %1 sym: %2").arg(
				   QString::number((int)state),
				   QString(data.at(*start))).toLatin1().constData());
		
		if (data.at(*start) == '\n')
			line++;

		// Skip eskape symbols
//		if (strchr(CJSON_ESCAPE_SYMBOLS, data.at(*start)))
//		{
//			continue;
//		}

		switch (data.at(*start))
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
							key += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
						}
						break;
						
					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();

					// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						state = ParsingStateValue;
						// TODO recursive! <<=
						type = CJsonElement::JsonTypeObject;
						ret.appendElement(
									new CJsonElement(
										key,
										QVariant::fromValue(parse(data, start)),
										CJsonElement::JsonTypeObject,
										this));
						break;

					// Value parsing process currently
					case ParsingStateValue:
						if (escape)
						{
							value += data.at(*start);
						}
						else
						{
							
						}						
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
						
				}
				break;

			// End of JSON object.
			case CJSON_RESERVED_ENDOBJECT:
				switch (state)
				{
					case ParsingStateWaitingKey:
						state = ParsingStateWaitingObject;
						return ret;
						break;


					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
						}
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(*start);
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
						return ret;
						break;						
					
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();

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
							key += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
						}
						break;
						
						// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						//value += data.at(*start);
						type = CJsonElement::JsonTypeArray;
						state = ParsingStateValue;
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						value += data.at(*start);
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
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
							key += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
						}
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							value += data.at(*start);
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							ret.appendElement(new CJsonElement(
												  key,
												  QVariant::fromValue(value),
												  CJsonElement::JsonTypeArray,
												  this));
							key.clear();
							value.clear();
						}
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
				}
				break;
				
			// Begin or end of string (if not escaped).
			case CJSON_RESERVED_STRING:
				switch (state)
				{
					case ParsingStateKey:
						if (escape)
						{
							key += data.at(*start);
						}
						else
							state = ParsingWaitingSeparatorValue;
						break;

					case ParsingStateWaitingKey:
						state = ParsingStateKey;
						break;

					case ParsingStateWaitingValue:
						type = CJsonElement::JsonTypeString;
						state = ParsingStateValue;
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += data.at(*start);
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							ret.appendElement(new CJsonElement(
												  key,
												  QVariant::fromValue(value),
												  CJsonElement::JsonTypeString,
												  this));
							key.clear();
							value.clear();
						}
						break;
						
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
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
							key += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
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
							value += data.at(*start);
						}
						else
						{
							emitParsingError(data.at(*start), line);
							return CJsonDoc();
						}
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
						
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
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
				}

				break;
			
			// Space
			case ' ':
				switch (state)
				{
					case ParsingStateKey:
						key += data.at(*start);
						break;
						
					case ParsingStateValue:
						key += data.at(*start);
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
						key += data.at(*start);
						break;
						
					// Value parsing process currently
					case ParsingStateValue:
						value += data.at(*start);
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(data.at(*start), line);
						return CJsonDoc();
				}
				break;

		}

		escape = data.at(*start) == CJSON_RESERVED_ESCAPE;
		(*start)++;
	}
	
	return ret;
}

CJsonDoc CJson::parse(QString data)
{
	int idx = 0;

	return parse(data.toLatin1(),
				 &idx);
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
