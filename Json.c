#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "Json.h"
#include "JsonObject.h"

static const char *string_start;
static PJsonObject object_start;
//function declaraiton
static const char *getItem(PJsonObject object,const char *jsonString);
static const char *getArrayItem(PJsonObject object,const char *jsonString);
static const char *getKey(PJsonObject object,const char *jsonString);
static const char *getObject(const char *jsonString,PJsonObject object);
static void showErrorMessage(const char *jsonString,ErrorType error);
static void freeChild(PJsonObject object);
static void freeArray(PJsonObject object);
static void printArray();
static void printChild();
static PJsonObject createNewItem();
static bool isInteger(const char *value);

PJsonObject jsonObject(const char *jsonString){
    PJsonObject object=createNewItem();
    string_start=jsonString;
    object_start=object;
    jsonParse(object,jsonString);
    return object;
}
const char *jsonParse(PJsonObject object,const char *jsonString){
    if(*jsonString=='{'){
        jsonString++;
        jsonString=getItem(object,jsonString);
    }else if(*jsonString=='['){
        jsonString++;
        object->type=ARRAY;
        jsonString=getArrayItem(object,jsonString);
    }else{
        showErrorMessage(jsonString,NO_START);
    }
    if(*jsonString!=']'&&*jsonString!='}')
        showErrorMessage(jsonString,NO_END);
    jsonString++;
    return jsonString;
}

static const char *getItem(PJsonObject object,const char *jsonString){
    if(*jsonString=='\"'){
        jsonString++;
        jsonString=getKey(object,jsonString);
        if(*jsonString==':'){
            jsonString++;
            jsonString=getObject(jsonString,object);
            if(*jsonString==','){
                jsonString++;
                object->next=createNewItem();
                jsonString=getItem(object->next,jsonString);
            }
        }else{
            //print error message that this is no ':'
            showErrorMessage(jsonString,NO_COLON);
        }
    }else{
        //print error message that string is not started with '"'.
        showErrorMessage(jsonString,NO_QUOTATION);
    }
	return jsonString;
}
static const char *getArrayItem(PJsonObject object,const char *jsonString){
    PJsonObject value=createNewItem();
    object->value_array=value;
    while(jsonString&&*jsonString!=']'){
        object->size++;
        jsonString=getObject(jsonString,value);
        if(*jsonString==','){
            jsonString++;
            value->value_array=createNewItem();
            value=value->value_array;
        }
    }
    return jsonString;
}
static const char *getKey(PJsonObject object,const char *jsonString){
    int size=0;
    const char *tmp=jsonString;
    char *key=NULL;
    while(*tmp!='\"'&&tmp){
        size++;
        tmp++;
    }
    size++;
    if(size==1){
        //print error message that there is no key.
        showErrorMessage(jsonString,NO_KEY);
    }else{
        char *value=(char*)malloc(size*sizeof(char));
        key=value;
        while(*jsonString!='\"'&&jsonString){
            *value=*jsonString;
            value++;
            jsonString++;
        }
        jsonString++;
        *value++='\0';
    }
    object->key=key;
    return jsonString;
}
static const char *getObject(const char *jsonString,PJsonObject object){
    int size=0;
    const char *tmp=jsonString;
    if(*jsonString=='\"'){
        object->type=STRING;
        tmp++;
        jsonString++;
        while(*tmp!='\"'&&tmp){
            size++;
            tmp++;
        }
        size++;
        if(size==1){
            //print error message that there is no key.
            showErrorMessage(jsonString,NO_VALUE);
            object->value_string=NULL;
        }else{
            char*value=(char*)malloc(size*sizeof(char));
            object->value_string=value;
            while(*jsonString!='\"'&&jsonString){
                *value=*jsonString;
                value++;
                jsonString++;
            }
            jsonString++;
            *value++='\0';
        }
    }else if(strncmp(jsonString,"true",4)==0||strncmp(jsonString,"false",5)==0){
        object->type=BOOL;
        if(strncmp(jsonString,"true",4)==0){
            object->value_boolean=true;
            jsonString+=4;
        }else{
            object->value_boolean=false;
            jsonString+=5;
        }
    }else if(*jsonString=='{'){
        object->type=OBJECT;
        object->value_child=createNewItem();
        jsonString=jsonParse(object->value_child,jsonString);
    }else if(*jsonString=='['){
        object->type=ARRAY;
        jsonString=jsonParse(object,jsonString);
    }else{
        while((*tmp!=','&&*tmp!=']'&&*tmp!='}')&&tmp){
            size++;
            tmp++;
        }
        size++;
        if(size==1){
            //print error message that there is no key.
            showErrorMessage(jsonString,NO_VALUE);
        }else{
            char *value=(char*)malloc(size*sizeof(char));
            char *t=value;
            while(size>1){
                *value=*jsonString;
                value++;
                jsonString++;
                size--;
            }
            *value++='\0';
            if(isInteger(t)==1){
                object->type=INTEGER;
                object->value_integer=atoi(t);
            }else{
                object->type=DOUBLE;
                object->value_double=strtod(t,NULL);
            }
        }
    }
    return jsonString;
}

static PJsonObject createNewItem(){
    PJsonObject object=(PJsonObject)malloc(sizeof(JsonObject));
    if (object) memset(object,0,sizeof(JsonObject));
    return object;
}
static bool isInteger(const char *value){
    while(value&&*value!='\0'){
        if(*value=='.')
            return false;
        value++;
    }
    return true;
}
char *getString(PJsonObject object,const char*key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==STRING){
           return tmp->value_string;
        }
        tmp=tmp->next;
    }
    return NULL;
}
bool getBoolean(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==BOOL){
           return tmp->value_boolean;
        }
        tmp=tmp->next;
    }
    return -1;
}
int getInteger(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==INTEGER){
           return tmp->value_integer;
        }
        tmp=tmp->next;
    }
    return INT_MIN;
}
double getDouble(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==DOUBLE){
           return tmp->value_double;
        }
        tmp=tmp->next;
    }
    return DBL_MIN;
}
float getFloat(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==DOUBLE){
           return (float)tmp->value_double;
        }
        tmp=tmp->next;
    }
    return FLT_MIN;
}
PJsonObject getJsonArray(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0&&tmp->type==ARRAY){
           return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
}
PJsonObject getJsonObject(PJsonObject object,const char *key){
    PJsonObject tmp=object;
    while(tmp){
        if(strcmp(tmp->key,key)==0){
           return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
}

int length(PJsonObject object){
    if(object->type=ARRAY){
        return object->size;
    }
    return -1;
}
char *getStringAt(PJsonObject object,int index){
    if(object->type!=ARRAY){
        //print error message that object is not array;
        return NULL;
    }
    PJsonObject tmp=object->value_array;
    for(;index>0&&tmp;tmp=tmp->value_array,index--);
    if(tmp->type=STRING)
        return tmp->value_string;
    else
        return NULL;
}
int getIntegerAt(PJsonObject object,int index){
    if(object->type!=ARRAY){
        //print error message that object is not array;
        return INT_MIN;
    }
    PJsonObject tmp=object->value_array;
    for(;index>0&&tmp;tmp=tmp->value_array,index--);
    if(tmp->type=INTEGER)
        return tmp->value_integer;
    else
        return INT_MIN;
}
double getDoubleAt(PJsonObject object,int index){
    if(object->type!=ARRAY){
        //print error message that object is not array;
        return DBL_MIN;
    }
    PJsonObject tmp=object->value_array;
    for(;index>0&&tmp;tmp=tmp->value_array,index--);
    if(tmp->type=DOUBLE)
        return tmp->value_double;
    else
        return DBL_MIN;
}
float getFloatAt(PJsonObject object,int index){
    if(object->type!=ARRAY){
        //print error message that object is not array;
        return FLT_MIN;
    }
    PJsonObject tmp=object->value_array;
    for(;index>0&&tmp;tmp=tmp->value_array,index--);
    if(tmp->type=DOUBLE)
        return (float)tmp->value_double;
    else
        return FLT_MIN;
}
PJsonObject getJsonObjectAt(PJsonObject object,int index){
    if(object->type!=ARRAY){
        //print error message that object is not array;
        return NULL;
    }
    PJsonObject tmp=object->value_array;
    for(;index>0&&tmp;tmp=tmp->value_array,index--);
    if(tmp->type=DOUBLE)
        return tmp;
    else
        return NULL;
}

static void freeChild(PJsonObject object){
    if(object){
        jsonFree(object);
    }
}
static void freeArray(PJsonObject object){
    PJsonObject tmp=object->value_array;
    while(object){
        if(object->type==OBJECT){
            jsonFree(object);
        }
        /**if(object->type=ARRAY){
            freeArray(object);
        }*/
        free(object);
        object=tmp;
        if(object)
            tmp=object->value_array;
    }
}
static void showErrorMessage(const char *jsonString,ErrorType error){
    int position=jsonString-string_start;
    if(error==NO_QUOTATION){
        printf("missing quotaion at position:%d\n%s\n",position,jsonString);
    }else if(error==NO_COLON){
        printf("missing colon at position:%d\n%s\n",position,jsonString);
    }else if(error==NO_KEY){
        printf("missing key of value at position:%d\n%s\n",position,jsonString);
    }else if(error==NO_VALUE){
        printf("missing value at position:%d\n%s\n",position,jsonString);
    }else if(error==NO_START){
        printf("missing start symbol at position:%d\n%s\n",position,jsonString);
    }else if(error==NO_END){
        printf("missing end symbol at position:%d\n%s\n",position,jsonString);
    }
    jsonFree(object_start);//release space
}
void jsonFree(PJsonObject object){
    PJsonObject tmp=object->next;
    while(object){
        if(object->value_array&&object->type==ARRAY){
            freeArray(object->value_array);
        }
        if(object->value_child&&object->type==OBJECT){
            freeChild(object->value_child);
        }
        free(object);
        object=tmp;
        if(object)
            tmp=object->next;
    }
}
static void printArray(PJsonObject object){
    while(object){
        switch(object->type){
            case BOOL:
                printf("%s\t",object->value_boolean==0?"false":"true");
                break;
            case DOUBLE:
                printf("%f\t",object->value_double);
                break;
            case INTEGER:
                 printf("%d\t",object->value_integer);
                break;
            case STRING:
                printf("\"%s\"\t",object->value_string);
                break;
            default:
                break;
        }
        object=object->value_array;
    }
    printf("\n");
}
static void printChild(PJsonObject object){
    if(object)
        jsonPrint(object);
}
void jsonPrint(PJsonObject object){
    while(object){
        if(object->value_array&&object->type==ARRAY){
            printArray(object);
        }
        if(object->value_child&&object->type==OBJECT){
            printf("%s:\n\t",object->key);
            printChild(object->value_child);
        }
        switch(object->type){
            case BOOL:
                printf("%s:%s\n",object->key,(object->value_boolean==0?"false":"true"));
                break;
            case DOUBLE:
                printf("%s:%f\n",object->key,object->value_double);
                break;
            case INTEGER:
                 printf("%s:%d\n",object->key,object->value_integer);
                break;
            case STRING:
                printf("%s:\"%s\"\n",object->key,object->value_string);
                break;
            default:
                break;
        }
        object=object->next;
    }
}
