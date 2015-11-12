#ifndef JSON_OBJECT
#define JSON_OBJECT
#define bool int
#define false 0
#define true 1
/**
 *type
 */
#define BOOL 1
#define INTEGER 2
#define DOUBLE 3
#define ARRAY 4
#define STRING 5
#define OBJECT 6
typedef struct JsonObject{
    /**
     *the key of the value;
     */
    char *key;
    /**
     *the type of the value;
     */
    int type;
    /**
     *array size
     */
    int size;
    /**
     *the space that saves value
     */
    char *value_string;
    int value_integer;
    double value_double;
    bool value_boolean;
    struct JsonObject *value_array;
    struct JsonObject *value_child;
    
    struct JsonObject *next,*prev;
}JsonObject,*PJsonObject;
#endif
