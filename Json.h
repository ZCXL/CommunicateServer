#ifndef JSON
#define JSON
#include "JsonObject.h"
#ifndef NULL
#define NULL 0
#endif
typedef enum ErrorType{
	NO_QUOTATION,NO_COLON,NO_KEY,NO_VALUE,NO_START,NO_END
}ErrorType;
/**
 *parse json string
 */
extern PJsonObject jsonObject(const char *jsonString);
extern const char *jsonParse(PJsonObject object,const char *jsonString);
/**
 *print json that is formatted
 */
extern void jsonPrint(PJsonObject object);
/**
 *free space that saves json object
 */
extern void jsonFree(PJsonObject object);
//get json value
extern char *getString(PJsonObject object,const char *key);
extern bool getBoolean(PJsonObject object,const char *key);
extern int getInteger(PJsonObject object,const char *key);
extern double getDouble(PJsonObject object,const char *key);
extern float getFloat(PJsonObject object,const char *key);
extern PJsonObject getJsonObject(PJsonObject object,const char *key);
extern PJsonObject getJsonArray(PJsonObject object,const char *key);
//get json value in array
#if 1
extern int length(PJsonObject object);
extern char *getStringAt(PJsonObject object,int index);
extern int getIntegerAt(PJsonObject object,int index);
extern double getDoubleAt(PJsonObject object,int index);
extern float getFloatAt(PJsonObject object,int index);
extern PJsonObject getJsonObjectAt(PJsonObject object,int index);
#endif
#endif
