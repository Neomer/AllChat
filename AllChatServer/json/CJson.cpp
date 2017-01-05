#include "CJson.h"

CJsonDoc CJson::fromFile(QString filename)
{
	QFile file(filename);

	if (!file.exists())
	{
		qDebug("Json: File not found!");
		return CJsonDoc();
	}

	if (!file.open(QFile::ReadOnly))
	{
		qDebug("Json: File access error!");
		return CJsonDoc();
	}

	return parse(file.readAll());
}

CJsonDoc CJson::parse(QByteArray data, int *start)
{
	CJsonDoc ret(this);
	ParsingState state = ParsingStateWaitingObject;
	QString key, value;
	CJsonElementValue::JsonValueType type;
	int line = 1;
	bool escape = false;

	qDebug("Start parsing %s from %d",
		   data.constData(), *start);

	key.reserve(1024);
	value.reserve(1024);

	while (*start < data.length())
	{
		char symbol = data.at(*start);


		if (symbol == '\n')
		{
			(*start)++;
			line++;
			continue;
		}

//		qDebug(tr("state: %1 sym: [%3] '%2'").arg(
//				   QString::number((int)state),
//				   QString(symbol),
//				   QString::number(symbol)).toLatin1().constData());
		

		// Skip eskape symbols
//		if (strchr(CJSON_ESCAPE_SYMBOLS, symbol))
//		{
//			continue;
//		}
		QVariant reqDoc;
		switch (symbol)
		{
			// Begin of JSON object '{'.
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
							key += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
							return CJsonDoc();
						}
						break;
						
					// Key parsed now waiting for a value separator ':'s
					case ParsingWaitingSeparatorValue:
						emitParsingError(symbol, line);
						return CJsonDoc();

					// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						state = ParsingStateValue;
						// TODO recursive! <<=
						type = CJsonElementValue::JsonTypeObject;
						reqDoc = QVariant::fromValue(parse(data, start));
						ret.appendElement(
									new CJsonElement(
										key,
										CJsonElementValue(reqDoc,
															CJsonElementValue::JsonTypeObject,
														  this),
										this));
						break;

					// Value parsing process currently
					case ParsingStateValue:
						if (escape)
						{
							value += symbol;
						}
						else
						{
							
						}						
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(symbol, line);
						return CJsonDoc();
						
				}
				break;

			// End of JSON object '}'.
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
							key += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
							return CJsonDoc();
						}
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += symbol;
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
						emitParsingError(symbol, line);
						return CJsonDoc();

				}
				break;
			
			// Begin of JSON array '['.
			case CJSON_RESERVED_BEGINARRAY:
				switch (state)
				{
						// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
							return CJsonDoc();
						}
						break;
						
						// Key name parsed now waiting for value
					case ParsingStateWaitingValue:
						// Value is object
						//value += symbol;
						type = CJsonElementValue::JsonTypeArray;
						state = ParsingStateValue;
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						value += symbol;
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(symbol, line);
						return CJsonDoc();
				}
				break;

			// End of JSON array ']' .
			case CJSON_RESERVED_ENDARRAY:
				switch (state)
				{
						// Key name parsing currently
					case ParsingStateKey:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							key += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
							return CJsonDoc();
						}
						break;
						
						// Value parsing process currently
					case ParsingStateValue:
						if ((escape)&&(CJSON_SETTINGS_ALLOWESCAPEDKEY))
						{
							value += symbol;
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							ret.appendElement(new CJsonElement(
												  key,
												  CJsonElementValue(
																QVariant::fromValue(value),
																CJsonElementValue::JsonTypeArray,
																this),
												  this));
							key.clear();
							value.clear();
						}
						break;
						
						// Value parsed now waiting for list separator ','
					default:
						emitParsingError(symbol, line);
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
							key += symbol;
						}
						else
							state = ParsingWaitingSeparatorValue;
						break;

					case ParsingStateWaitingKey:
						state = ParsingStateKey;
						break;

					case ParsingStateWaitingValue:
						type = CJsonElementValue::JsonTypeString;
						state = ParsingStateValue;
						break;

					case ParsingStateValue:
						if (escape)
						{
							value += symbol;
						}
						else
						{
							state = ParsingWaitingList;
							qDebug(tr("Key: %1 Value: %2 Type: %3").arg(
									   key, value, QString::number(type)).toLatin1().constData());
							ret.appendElement(new CJsonElement(
												  key,
												  CJsonElementValue(
																QVariant::fromValue(value),
																CJsonElementValue::JsonTypeString,
																this),
												  this));
							key.clear();
							value.clear();
						}
						break;
						
					default:
						emitParsingError(symbol, line);
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
							key += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
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
							value += symbol;
						}
						else
						{
							emitParsingError(symbol, line);
							return CJsonDoc();
						}
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(symbol, line);
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
						emitParsingError(symbol, line);
						return CJsonDoc();
				}

				break;
			
			// Space
			case ' ': case '\t': case '\n': case '\r':
				switch (state)
				{
					case ParsingStateKey:
						key += symbol;
						break;
						
					case ParsingStateValue:
						value += symbol;
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
						key += symbol;
						break;
						
					// Value parsing process currently
					case ParsingStateValue:
						value += symbol;
						break;
					
					// Value parsed now waiting for list separator ','
					default:
						emitParsingError(symbol, line);
						return CJsonDoc();
				}
				break;

		}

		escape = symbol == CJSON_RESERVED_ESCAPE;
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
	
	emit parsingFailed(-1, tr("Unexpected symbol [%3] '%1' at line %2").arg(
						   QString(symbol),
						   QString::number(line),
						   QString::number(symbol)));
}
