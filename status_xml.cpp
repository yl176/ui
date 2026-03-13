#include <QCoreApplication>
//#include <QtXml>
//也可以include <QDomDocument>
#include <QDomDocument>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "status_xml.h"
#include <QDebug>
#include"common.h"
#if defined(__linux)&&defined(__x86_64)
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#define xmlFileNameT "/home/orchid/qt/qwid/status1.xml"
#endif

//写xml
void WriteXml()
{
    //打开或创建文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
        return;

    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction; //添加处理命令
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //添加根节点
    QDomElement root=doc.createElement("library");
    doc.appendChild(root);
    //添加第一个子节点及其子元素
    QDomElement book=doc.createElement("book");
    book.setAttribute("id",1); //方式一：创建属性  其中键值对的值可以是各种类型
    QDomAttr time=doc.createAttribute("time"); //方式二：创建属性 值必须是字符串
    time.setValue("2013/6/13");
    book.setAttributeNode(time);
    QDomElement title=doc.createElement("title"); //创建子元素
    QDomText text; //设置括号标签中间的值
    text=doc.createTextNode("C++ primer");
    book.appendChild(title);
    title.appendChild(text);
    QDomElement author=doc.createElement("author"); //创建子元素
    text=doc.createTextNode("Stanley Lippman");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    //添加第二个子节点及其子元素，部分变量只需重新赋值
    book=doc.createElement("book");
    book.setAttribute("id",2);
    time=doc.createAttribute("time");
    time.setValue("2007/5/25");
    book.setAttributeNode(time);
    title=doc.createElement("title");
    text=doc.createTextNode("Thinking in Java");
    book.appendChild(title);
    title.appendChild(text);
    author=doc.createElement("author");
    text=doc.createTextNode("Bruce Eckel");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

}

//读xml
void ReadXml()
{
    //打开或创建文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement(); //返回根节点
    qDebug()<<root.nodeName();
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            qDebug()<<e.tagName()<<" "<<e.attribute("id")<<" "<<e.attribute("time"); //打印键值对，tagName和nodeName是一个东西
            QDomNodeList list=e.childNodes();
            for(int i=0;i<list.count();i++) //遍历子元素，count和size都可以用,可用于标签数计数
            {
                QDomNode n=list.at(i);
                if(node.isElement())
                    qDebug()<<n.nodeName()<<":"<<n.toElement().text();
            }
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }

}

//增加xml内容
void AddXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //增加一个一级子节点以及元素
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomElement book=doc.createElement("book");
    book.setAttribute("id",3);
    book.setAttribute("time","1813/1/27");
    QDomElement title=doc.createElement("title");
    QDomText text;
    text=doc.createTextNode("Pride and Prejudice");
    title.appendChild(text);
    book.appendChild(title);
    QDomElement author=doc.createElement("author");
    text=doc.createTextNode("Jane Austen");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

//删减xml内容
void RemoveXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //删除一个一级子节点及其元素，外层节点删除内层节点于此相同
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();  //一定要记得关掉啊，不然无法完成操作

    QDomElement root=doc.documentElement();
    QDomNodeList list=doc.elementsByTagName("book"); //由标签名定位
    for(int i=0;i<list.count();i++)
    {
        QDomElement e=list.at(i).toElement();
        if(e.attribute("time")=="2007/5/25")  //以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
            root.removeChild(list.at(i));
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

//更新xml内容
void UpdateXml()
{
    //打开文件
    QFile file("test.xml"); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomNodeList list=root.elementsByTagName("book");
    QDomNode node=list.at(list.size()-1).firstChild(); //定位到第三个一级子节点的子元素
    QDomNode oldnode=node.firstChild(); //标签之间的内容作为节点的子节点出现,当前是Pride and Projudice
    node.firstChild().setNodeValue("Emma");
    QDomNode newnode=node.firstChild();
    node.replaceChild(newnode,oldnode);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

int test(int argc, char *argv[])
{

    qDebug()<<"write xml to file...";
    WriteXml();
    qDebug()<<"read xml to display...";
    ReadXml();
    qDebug()<<"add contents to xml...";
    AddXml();
    qDebug()<<"remove contents from xml...";
    RemoveXml();
    qDebug()<<"update contents to xml...";
    UpdateXml();
    return 0;

}

CStatusXml::CStatusXml(const char * fileName)
{
    file.setFileName(fileName);
    if(!file.exists())
    {
       bool openRet = file.open(QIODevice::WriteOnly|QIODevice::Text);
       if(openRet == false)
       {
            qDebug()<<"create xml file failed"<<endl;
       }
       file.close();

	   init_status();
    }
    else
    {
        //clean the content here
    }
    roomIndex = 0;
    isReadOut = 1;    

}

void CStatusXml::init_status(void)
{
    QDomDocument doc;
    //写入xml头部

    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
        return;


    QDomProcessingInstruction instruction; //添加处理命令
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("status");
    doc.appendChild(root);

    QDomElement rooms=doc.createElement("rooms");
    root.appendChild(rooms);

    QDomElement room=doc.createElement("room");
    room.setAttribute("id",0);

    QDomAttr attr1 = room.attributeNode("id");
    QString attrName1 = attr1.name();
    QString attrVal1= attr1.value();
    //qDebug<<qPrintable(attrName1)<<"="<<qPrintable(attrVal1)<<endl;

    rooms.appendChild(room);

    QDomElement light=doc.createElement("light");
    room.appendChild(light);

    QDomElement scenes=doc.createElement("scenes");
    room.appendChild(scenes);

    QDomElement curtains=doc.createElement("curtains");
    room.appendChild(curtains);

    QDomElement havcs=doc.createElement("havcs");
    room.appendChild(havcs);


    room=doc.createElement("room");
    room.setAttribute("id",1);
    rooms.appendChild(room);

    light=doc.createElement("light");
    room.appendChild(light);

    scenes=doc.createElement("scenes");
    room.appendChild(scenes);

    curtains=doc.createElement("curtains");
    room.appendChild(curtains);

    havcs=doc.createElement("havcs");
    room.appendChild(havcs);

    QDomElement outdoor=doc.createElement("outdoor");
    root.appendChild(outdoor);

    QDomElement temperature=doc.createElement("temperature");
    outdoor.appendChild(temperature);

    QDomElement aqi=doc.createElement("aqi");
    outdoor.appendChild(aqi);

    QDomElement weather=doc.createElement("weather");
    outdoor.appendChild(weather);

    QDomElement openDev=doc.createElement("openDev");
    root.appendChild(openDev);

    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

}
#if defined(__linux)&&defined(__x86_64)
void init_status1(void)
{
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

    xmlNodePtr status = xmlNewNode(NULL, BAD_CAST"status");

    xmlDocSetRootElement(doc, status);

    xmlNodePtr rooms_node = xmlNewNode(NULL,BAD_CAST"rooms");
    xmlAddChild(status,rooms_node);

    xmlNodePtr outdoor_node = xmlNewNode(NULL,BAD_CAST"outdoor");
    xmlAddChild(status,outdoor_node);

    xmlNodePtr node_room = xmlNewNode(NULL,BAD_CAST"room");
    xmlNewProp(node_room,BAD_CAST"id",BAD_CAST "0");
    xmlAddChild(rooms_node,node_room);

    xmlNodePtr node_light = xmlNewNode(NULL,BAD_CAST"light");
    xmlNodePtr node_scenes = xmlNewNode(NULL,BAD_CAST"scenes");
    xmlNodePtr node_curtains = xmlNewNode(NULL,BAD_CAST"curtains");
    xmlNodePtr node_havcs = xmlNewNode(NULL,BAD_CAST"havcs");
    xmlAddChild(node_room,node_light);
    xmlAddChild(node_room,node_scenes);
    xmlAddChild(node_room,node_curtains);
    xmlAddChild(node_room,node_havcs);


    node_room = xmlNewNode(NULL,BAD_CAST"room");
    xmlNewProp(node_room,BAD_CAST"id",BAD_CAST "1");
    xmlAddChild(rooms_node,node_room);

    node_light = xmlNewNode(NULL,BAD_CAST"light");
    node_scenes = xmlNewNode(NULL,BAD_CAST"scenes");
    node_curtains = xmlNewNode(NULL,BAD_CAST"curtains");
    node_havcs = xmlNewNode(NULL,BAD_CAST"havcs");
    xmlAddChild(node_room,node_light);
    xmlAddChild(node_room,node_scenes);
    xmlAddChild(node_room,node_curtains);
    xmlAddChild(node_room,node_havcs);

    xmlNodePtr node_temp = xmlNewNode(NULL,BAD_CAST"temperature");
    xmlNodePtr node_aqi = xmlNewNode(NULL,BAD_CAST"aqi");
    xmlNodePtr node_weather = xmlNewNode(NULL,BAD_CAST"weather");
    xmlAddChild(outdoor_node,node_temp);
    xmlAddChild(outdoor_node,node_aqi);
    xmlAddChild(outdoor_node,node_weather);

    //xmlSaveFile will create the file if it is not exist
    //int ret = xmlSaveFile(xmlFileNameT,doc);
    int ret = xmlSaveFormatFile (xmlFileNameT, doc, 1);

    if(ret==-1)
    {
        printf("save xml to file failed");
    }

    if(doc)
    {
        xmlFreeDoc(doc);
        doc = NULL;
    }

}
#endif
void CStatusXml::updateInDoor(int enumIn,unsigned int val)
{
    return;
    if(!file.open(QFile::ReadOnly))
        return;
	
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOutDoorNew = 0;
    int flagNeedNewEle = 0;

    QDomNode indoor = status.namedItem("indoor");
    QDomElement indoorEle;
    if(indoor.isNull() == 1)
    {
        indoorEle=doc.createElement("indoor");
        status.appendChild(indoorEle);

        flagOutDoorNew = 1;
    }
    else
    {
        indoorEle = indoor.toElement();
        flagOutDoorNew = 0;

    }

    QString strKey;
    if(enumIn == CHAR_P2_TEMP_SENSOR)
    {
        strKey = "temp";
    }
    else if(enumIn==CHAR_P2_HUMIDITY_SENSOR)
    {
        strKey = "hum";
    }
    else if(enumIn==CHAR_P2_VOC_SENSOR)
    {
        strKey = "voc";
    }
    QDomNode nodeToProc,oldNode;
    QDomElement eleToProc;

    if(flagOutDoorNew==0)
    {
        nodeToProc = indoor.namedItem(strKey);
        if(nodeToProc.isNull() == 1)
        {
           flagNeedNewEle = 1;
        }
        else
        {
           oldNode = nodeToProc;
        }
    }


    eleToProc = doc.createElement(strKey);
    eleToProc.setAttribute("value",val);


    if((flagNeedNewEle==1)||(flagOutDoorNew==1))
    {
        indoor.appendChild(eleToProc);
    }
    else
    {
        indoor.replaceChild(eleToProc,oldNode);
    }
//    QDomNode nodeTest = outdoor.namedItem("temperature");
//    QDomAttr attr = nodeTest.toElement().attributeNode("value");
//    QString attrName = attr.name();
//    QString attrVal= attr.value();

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void CStatusXml::updateOutDoor(int enumIn,unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOutDoorNew = 0;
    int flagNeedNewEle = 0;

    QDomNode outdoor = status.namedItem("outdoor");
    QDomElement outdoorEle;
    if(outdoor.isNull() == 1)
    {
        outdoorEle=doc.createElement("outdoor");
        status.appendChild(outdoorEle);

        flagOutDoorNew = 1;
    }
    else
    {
        outdoorEle = outdoor.toElement();
        flagOutDoorNew = 0;

    }

    QString strKey;
    if(enumIn == CHAR_P2_OUTDOOR_TEMP)
    {
        strKey = "temperature";
    }
    else if(enumIn==CHAR_P2_OUTDOOR_AQI)
    {
        strKey = "aqi";
    }
    else if(enumIn==CHAR_P2_OUTDOOR_WEATHER)
    {
        strKey = "weather";
    }
    QDomNode nodeToProc,oldNode;
    QDomElement eleToProc;

    if(flagOutDoorNew==0)
    {
        nodeToProc = outdoor.namedItem(strKey);
        if(nodeToProc.isNull() == 1)
        {
           flagNeedNewEle = 1;
        }
        else
        {
           oldNode = nodeToProc;
        }
    }


    eleToProc = doc.createElement(strKey);
    eleToProc.setAttribute("value",val);


    if((flagNeedNewEle==1)||(flagOutDoorNew==1))
    {
        outdoor.appendChild(eleToProc);
    }
    else
    {
        outdoor.replaceChild(eleToProc,oldNode);
    }
//    QDomNode nodeTest = outdoor.namedItem("temperature");
//    QDomAttr attr = nodeTest.toElement().attributeNode("value");
//    QString attrName = attr.name();
//    QString attrVal= attr.value();

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

#if defined(__linux)&&defined(__x86_64)

xmlXPathObjectPtr getNodeset(xmlDocPtr doc, const xmlChar *xpath)
{
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;
    context = xmlXPathNewContext(doc);

    if (context == NULL) {
        printf("context is NULL\n");
        return NULL;
    }

    result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (result == NULL) {
        printf("xmlXPathEvalExpression return NULL\n");
        return NULL;
    }

    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        xmlXPathFreeObject(result);
        printf("nodeset is empty\n");
        return NULL;
    }

    return result;
}

void updateOutDoor1(int enumIn,unsigned int val)
{

    xmlDocPtr doc = xmlReadFile(xmlFileNameT,"utf-8",XML_PARSE_RECOVER);
    xmlNodePtr rootNode=NULL;
    xmlNodePtr curNode=NULL;
    if(doc==NULL)
    {
        return;
    }

    rootNode = xmlDocGetRootElement(doc); //确定文档根元素

    if (NULL == rootNode)
    {
       //fprintf(stderr,"empty document\n");
       xmlFreeDoc(doc);
       return;
    }
    curNode = rootNode;
    if (xmlStrcmp(curNode->name, BAD_CAST "status"))
    {
       //fprintf(stderr,"document of the wrong type, root node != root");
       xmlFreeDoc(doc);
       return;
    }

    xmlChar strPath[128];

    if(enumIn == CHAR_P2_OUTDOOR_TEMP)
    {
         sprintf((char*)strPath,"%s%s","/status/outdoor","/temperature");
    }
    else if(enumIn==CHAR_P2_OUTDOOR_AQI)
    {
         sprintf((char*)strPath,"%s%s","/status/outdoor","/aqi");
    }
    else if(enumIn==CHAR_P2_OUTDOOR_WEATHER)
    {
        sprintf((char*)strPath,"%s%s","/status/outdoor","/weather");
    }


    xmlXPathObjectPtr ptrPathObj = getNodeset(doc,(xmlChar*)"/status/outdoor");


}
#endif
void CStatusXml::updateOpenDev(unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOpenDevNew = 0;

    QDomNode openDev = status.namedItem("openDev");
    QDomElement openDevEle,oldEle;
    if(openDev.isNull() == 1)
    {
        flagOpenDevNew = 1;
    }
    else
    {
        oldEle = openDev.toElement();
        flagOpenDevNew = 0;

    }

    openDevEle=doc.createElement("openDev");
    openDevEle.setAttribute("bitmap",val);

    if(flagOpenDevNew)
    {
       status.appendChild(openDevEle);
    }
    else
    {
        status.replaceChild(openDevEle,oldEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}
void CStatusXml::updateSetup(unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOpenDevNew = 0;

    QDomNode openDev = status.namedItem("setup");
    QDomElement openDevEle,oldEle;
    if(openDev.isNull() == 1)
    {
        flagOpenDevNew = 1;
    }
    else
    {
        oldEle = openDev.toElement();
        flagOpenDevNew = 0;

    }

    openDevEle=doc.createElement("setup");
    openDevEle.setAttribute("netstatus",val);

    if(flagOpenDevNew)
    {
       status.appendChild(openDevEle);
    }
    else
    {
        status.replaceChild(openDevEle,oldEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void CStatusXml::updateSsid(char *p_ap_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOpenDevNew = 0;

    QDomNode openDev = status.namedItem("wifi");
    QDomElement openDevEle,oldEle;
    if(openDev.isNull() == 1)
    {
        flagOpenDevNew = 1;
    }
    else
    {
        oldEle = openDev.toElement();
        flagOpenDevNew = 0;

    }

    openDevEle=doc.createElement("wifi");
    openDevEle.setAttribute("ssid", p_ap_name);

    if(flagOpenDevNew)
    {
       status.appendChild(openDevEle);
    }
    else
    {
        status.replaceChild(openDevEle,oldEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    
    QTextStream out_stream(&file);
    doc.save(out_stream,4); 
    file.close();
}

void CStatusXml::updateip(char *p_ip_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOpenDevNew = 0;

    QDomNode openDev = status.namedItem("dev_ip");
    QDomElement openDevEle,oldEle;
    if(openDev.isNull() == 1)
    {
        flagOpenDevNew = 1;
    }
    else
    {
        oldEle = openDev.toElement();
        flagOpenDevNew = 0;

    }

    openDevEle=doc.createElement("dev_ip");
    openDevEle.setAttribute("ip", p_ip_name);

    if(flagOpenDevNew)
    {
       status.appendChild(openDevEle);
    }
    else
    {
        status.replaceChild(openDevEle,oldEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    
    QTextStream out_stream(&file);
    doc.save(out_stream,4); 
    file.close();
}


void CStatusXml::updatedb(char *p_db_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    int flagOpenDevNew = 0;

    QDomNode openDev = status.namedItem("wifi_db");
    QDomElement openDevEle,oldEle;
    if(openDev.isNull() == 1)
    {
        flagOpenDevNew = 1;
    }
    else
    {
        oldEle = openDev.toElement();
        flagOpenDevNew = 0;

    }

    openDevEle=doc.createElement("wifi_db");
    openDevEle.setAttribute("db", p_db_name);

    if(flagOpenDevNew)
    {
       status.appendChild(openDevEle);
    }
    else
    {
        status.replaceChild(openDevEle,oldEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    
    QTextStream out_stream(&file);
    doc.save(out_stream,4); 
    file.close();
}

void CStatusXml::setRoomIndex(int roomI)
{
    roomIndex = roomI;
}

void CStatusXml::updateDevInfo(int devType, unsigned int devId,unsigned int cid,unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode nodeRooms = status.namedItem("rooms");

    if(nodeRooms.isNull())
    {
        return;
    }

    QDomNodeList roomList=nodeRooms.toElement().elementsByTagName("room");
    QDomElement roomEle;
    QDomElement devEle,devsEle;
    QDomElement charactorEle;

    QDomNode roomNode,devNode,charNode,devsNode;

    QDomNodeList charList;
    QDomNodeList devList;

    QString strDevs[2] ={"curtains","havcs"};
    QString strDev[2] ={"curtain","havc"};
    int flagRoomFound = 0, flagDevsFound = 0,flagDevFound=0,flagCharFound=0;
    if(roomList.count() == 0)
    {
       flagRoomFound = 0;
    }
    else
    {
        QDomAttr attr;
        for(int i=0;i<roomList.count();i++)
        {
            roomNode =roomList.at(i);
            roomEle = roomNode.toElement();
            attr = roomEle.attributeNode("id");
            if(attr.isNull()==false)
            {
                if(attr.value() == QString::number(roomIndex,10))
                {
                    flagRoomFound = 1;
                    break;
                }
            }
        }

        if(flagRoomFound == 1)
        {
             devsNode = roomNode.namedItem(strDevs[devType]);

             if(devsNode.isNull()==0)
             {//devs is foundsss
                //devEle =devNode.toElement();
                flagDevsFound = 1;
                devList = devsNode.toElement().elementsByTagName(strDev[devType]);

                for(int i=0;i<devList.count();i++)
                {
                    devNode = devList.at(i);
                    devEle = devNode.toElement();
                    attr = devEle.attributeNode("devId");
                    if(attr.isNull()==false)
                    {
                        if(attr.value() == QString::number(devId,10))
                        {
                            flagDevFound = 1;
                            break;
                        }
                    }
                }

                if(flagDevFound == 1)
                {
                    charList = devNode.toElement().elementsByTagName("char");
                    QString strCid;
                    if(charList.count()>0)
                    {
                        for(int i=0;i<charList.count();i++)
                        {
                            charNode =charList.at(i);
                            strCid = charNode.toElement().attribute("cid");

                            if(!strCid.isEmpty()
                                    && strCid == QString::number(cid,16))
                            {
                                //char is found
                                flagCharFound = 1;
                                break;
                            }
                        }
                    }
                }
             }
             else
             {//devs is not founds
                 flagDevsFound = 0;
                 flagDevFound = 0;
                 flagCharFound = 0;
             }
        }

    }

    if(flagRoomFound)
    {
        roomEle = roomNode.toElement();
    }
    else
    {
        roomEle = doc.createElement("room");
        roomEle.setAttribute("id",roomIndex);
        nodeRooms.appendChild(roomEle);
    }

    if(flagDevsFound)
    {
        devsEle = devsNode.toElement();
    }
    else
    {
        devsEle = doc.createElement(strDevs[devType]);
        roomEle.appendChild(devsEle);
    }

    if(flagDevFound)
    {
        devEle = devNode.toElement();
    }
    else
    {
        devEle = doc.createElement(strDev[devType]);
        devEle.setAttribute("devId",devId);
        devsEle.appendChild(devEle);
    }

    charactorEle = doc.createElement("char");
    charactorEle.setAttribute("cid",QString::number(cid,16));
    charactorEle.setAttribute("value",val);
    if(flagCharFound)
    {
        devEle.replaceChild(charactorEle,charNode);
    }
    else
    {
        devEle.appendChild(charactorEle);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

}

void CStatusXml::updateScene(unsigned int scenceId,unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode nodeRooms = status.namedItem("rooms");

    if(nodeRooms.isNull())
    {
        return;
    }

    QDomNodeList roomList=nodeRooms.toElement().elementsByTagName("room");

    QDomNode roomNode,scenesNode,sceneNode;
    QDomElement roomEle,scenesEle,sceneEle;

    QDomNodeList sceneList;
    QDomAttr attr;
    int flagScenesFound = 0,flagSceneFound=0,flagRoomFound=0;
    if(roomList.count() == 0)
    {
       flagRoomFound = 0;
    }
    else
    {

        for(int i=0;i<roomList.count();i++)
        {
            roomNode =roomList.at(i);
            roomEle = roomNode.toElement();
            attr = roomEle.attributeNode("id");
            if(attr.isNull()==false)
            {
                if(attr.value() == QString::number(roomIndex,10))
                {
                    flagRoomFound = 1;
                    break;
                }
            }
        }

        if(flagRoomFound==1)
        {
            scenesNode = roomEle.namedItem("scenes");

            if(scenesNode.isNull())
            {
                flagScenesFound = 0;
            }
            else
            {
                flagScenesFound = 1;
                scenesEle = scenesNode.toElement();
                sceneList = scenesEle.elementsByTagName("scene");

                for(int i=0;i<sceneList.count();i++)
                {
                    sceneNode = sceneList.at(i);
                    sceneEle = sceneNode.toElement();
                    attr = sceneEle.attributeNode("id");
                    if(attr.value()==QString::number(scenceId,10))
                    {
                        flagSceneFound = 1;
                        break;
                    }
                }

            }
        }

    }


    if(flagRoomFound == 0)
    {
        roomEle = doc.createElement("room");
        roomEle.setAttribute("id",roomIndex);
        nodeRooms.appendChild(roomEle);
    }

    if(flagScenesFound == 0)
    {
        scenesEle = doc.createElement("scenes");
        roomEle.appendChild(scenesEle);
    }

    sceneEle = doc.createElement("scene");
    sceneEle.setAttribute("id",scenceId);
    sceneEle.setAttribute("status",val);

    if(flagSceneFound==0)
    {
        scenesEle.appendChild(sceneEle);
    }
    else
    {
        scenesEle.replaceChild(sceneEle,sceneNode);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void CStatusXml::updateLight(unsigned int val)
{
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode nodeRooms = status.namedItem("rooms");

    if(nodeRooms.isNull())
    {
        return;
    }

    QDomNodeList roomList=nodeRooms.toElement().elementsByTagName("room");

    QDomNode roomNode,lightNode;
    QDomElement roomEle,lightEle;

    QDomAttr attr;
    int flagRoomFound=0,flagLightFound=0;
    if(roomList.count() == 0)
    {
       flagRoomFound = 0;
    }
    else
    {

        for(int i=0;i<roomList.count();i++)
        {
            roomNode =roomList.at(i);
            roomEle = roomNode.toElement();
            attr = roomEle.attributeNode("id");
            if(attr.isNull()==false)
            {
                if(attr.value() == QString::number(roomIndex,10))
                {
                    flagRoomFound = 1;
                    break;
                }
            }
        }

        if(flagRoomFound==1)
        {
            lightNode = roomEle.namedItem("light");

            if(lightNode.isNull())
            {
                flagLightFound = 0;
            }
            else
            {
                flagLightFound = 1;

            }
        }

    }


    if(flagRoomFound == 0)
    {
        roomEle = doc.createElement("room");
        roomEle.setAttribute("id",roomIndex);
        nodeRooms.appendChild(roomEle);
    }

    lightEle = doc.createElement("light");
    lightEle.setAttribute("status",val);

    if(flagLightFound==0)
    {
        roomEle.appendChild(lightEle);
    }
    else
    {
        roomEle.replaceChild(lightEle,lightNode);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void CStatusXml::updateLightDim(unsigned int val)
{
	qDebug()<<"update light dim start\n";

    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode nodeRooms = status.namedItem("rooms");

    if(nodeRooms.isNull())
    {
        return;
    }

    QDomNodeList roomList=nodeRooms.toElement().elementsByTagName("room");

    QDomNode roomNode,lightNode;
    QDomElement roomEle,lightEle;

    QDomAttr attr;
    int flagRoomFound=0,flagLightFound=0;
    if(roomList.count() == 0)
    {
       flagRoomFound = 0;
    }
    else
    {

        for(int i=0;i<roomList.count();i++)
        {
            roomNode =roomList.at(i);
            roomEle = roomNode.toElement();
            attr = roomEle.attributeNode("id");
            if(attr.isNull()==false)
            {
                if(attr.value() == QString::number(roomIndex,10))
                {
                    flagRoomFound = 1;
                    break;
                }
            }
        }

        if(flagRoomFound==1)
        {
            lightNode = roomEle.namedItem("light");

            if(lightNode.isNull())
            {
                flagLightFound = 0;
            }
            else
            {
                flagLightFound = 1;

            }
        }

    }


    if(flagRoomFound == 0)
    {
        roomEle = doc.createElement("room");
        roomEle.setAttribute("id",roomIndex);
        nodeRooms.appendChild(roomEle);
    }

    lightEle = doc.createElement("light");
    lightEle.setAttribute("dim",val);

    if(flagLightFound==0)
    {
        roomEle.appendChild(lightEle);
    }
    else
    {
        roomEle.replaceChild(lightEle,lightNode);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    
    file.close();

	qDebug()<<"update light dim end\n";
}

void CStatusXml::setReadOut(int flag)
{
    isReadOut = flag;
}
int  CStatusXml::getReadOut(void)
{
    return isReadOut;
}

void CStatusXml::getOutDoorValues(char* bitMapOut,OutDoorValues*pstOutDoorValue)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode outdoor = status.namedItem("outdoor");
    char bitMapTemp = 0;
    if(outdoor.isNull() == 1)
    {
        return;
    }
    else
    {
        QDomNode node=outdoor.firstChild();
        QDomElement eleToProc;
        QString strTag;
        QDomAttr attr;
        while(!node.isNull())
        {
             eleToProc= node.toElement();

             strTag = eleToProc.nodeName();
             attr = eleToProc.attributeNode("value");
             if(strTag == QString("temperature") && !attr.isNull())
             {
                 bitMapTemp |=BIT_TEMP;
                 pstOutDoorValue->temperature = attr.value().toInt();
             }
             else if(strTag == QString("aqi") && !eleToProc.attributeNode("value").isNull())
             {
                 bitMapTemp |=BIT_AQI;
                 pstOutDoorValue->aqi = attr.value().toInt();
             }
             else if(strTag == QString("weather") && !eleToProc.attributeNode("value").isNull())
             {
                 bitMapTemp |=BIT_WEATHER;
                 pstOutDoorValue->weather = attr.value().toInt();
             }

             node = node.nextSibling();
        }

    }
    *bitMapOut = bitMapTemp;

}

void CStatusXml::getInDoorValues(char* bitMapOut, InDoorValues*pstInDoorValue)
{
    return;
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode indoor = status.namedItem("indoor");
    char bitMapTemp = 0;
    if(indoor.isNull() == 1)
    {
        return;
    }
    else
    {
        QDomNode node=indoor.firstChild();
        QDomElement eleToProc;
        QString strTag;
        QDomAttr attr;
        while(!node.isNull())
        {
             eleToProc= node.toElement();

             strTag = eleToProc.nodeName();
             attr = eleToProc.attributeNode("value");
             if(strTag == QString("temp") && !attr.isNull())
             {
                 bitMapTemp |= BIT_TEMP_IN;
                 pstInDoorValue->temp = attr.value().toInt();
             }
             else if(strTag == QString("aqi") && !eleToProc.attributeNode("value").isNull())
             {
                 bitMapTemp |= BIT_HUM_IN;
                 pstInDoorValue->hum = attr.value().toInt();
             }
             else if(strTag == QString("weather") && !eleToProc.attributeNode("value").isNull())
             {
                 bitMapTemp |= BIT_VOC_IN;
                 pstInDoorValue->voc = attr.value().toInt();
             }

             node = node.nextSibling();
        }

    }
    *bitMapOut = bitMapTemp;

}

void CStatusXml::getSetup(unsigned int * bitMapOut)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode opendev = status.namedItem("setup");
    QDomElement opendevEle;
    QDomAttr attr;
    unsigned int bitMapTemp = 0;
    if(opendev.isNull() == 1)
    {
        return;
    }
    else
    {
        opendevEle = opendev.toElement();
        attr = opendevEle.attributeNode("netstatus");
        if(!attr.isNull())
        {
            bitMapTemp = attr.value().toInt();
        }
    }
    *bitMapOut = bitMapTemp;
}
void CStatusXml::getSsid(char *pp_ap_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode opendev = status.namedItem("wifi");
    QDomElement opendevEle;
    QDomAttr attr;
    unsigned int bitMapTemp = 0;
    if(opendev.isNull() == 1)
    {
        return;
    }
    else
    {
        opendevEle = opendev.toElement();
        attr = opendevEle.attributeNode("ssid");
        if(!attr.isNull())
        {
	        char*  ch;
			QByteArray ba = attr.value().toLatin1(); // must
			ch = ba.data();
            strcpy(pp_ap_name, ch);
        }
		else
		{
			strcpy(pp_ap_name, "unknown");
		}
    }
}
void CStatusXml::getip(char *pp_ip_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //鏇存柊涓?涓爣绛鹃」,濡傛灉鐭ラ亾xml鐨勭粨鏋勶紝鐩存帴瀹氫綅鍒伴偅涓爣绛句笂瀹氱偣鏇存柊
    //鎴栬?呯敤閬嶅巻鐨勬柟娉曞幓鍖归厤tagname鎴栬?卆ttribut锛寁alue鏉ユ洿鏂?
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode opendev = status.namedItem("dev_ip");
    QDomElement opendevEle;
    QDomAttr attr;
    unsigned int bitMapTemp = 0;
    if(opendev.isNull() == 1)
    {
        return;
    }
    else
    {
        opendevEle = opendev.toElement();
        attr = opendevEle.attributeNode("ip");
        if(!attr.isNull())
        {
	        char*  ch;
			QByteArray ba = attr.value().toLatin1(); // must
			ch = ba.data();
            strcpy(pp_ip_name, ch);
        }
		else
		{
			strcpy(pp_ip_name, "null");
		}
    }
}
void CStatusXml::getdb(char *pp_db_name)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //鏇存柊涓?涓爣绛鹃」,濡傛灉鐭ラ亾xml鐨勭粨鏋勶紝鐩存帴瀹氫綅鍒伴偅涓爣绛句笂瀹氱偣鏇存柊
    //鎴栬?呯敤閬嶅巻鐨勬柟娉曞幓鍖归厤tagname鎴栬?卆ttribut锛寁alue鏉ユ洿鏂?
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode opendev = status.namedItem("wifi_db");
    QDomElement opendevEle;
    QDomAttr attr;
    unsigned int bitMapTemp = 0;
    if(opendev.isNull() == 1)
    {
        return;
    }
    else
    {
        opendevEle = opendev.toElement();
        attr = opendevEle.attributeNode("db");
        if(!attr.isNull())
        {
	        char*  ch;
			QByteArray ba = attr.value().toLatin1(); // must
			ch = ba.data();
            strcpy(pp_db_name, ch);
        }
		else
		{
			strcpy(pp_db_name, "null");
		}
    }
}

void CStatusXml::getOpenDev(unsigned int * bitMapOut)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }


    QDomNode opendev = status.namedItem("openDev");
    QDomElement opendevEle;
    QDomAttr attr;
    unsigned int bitMapTemp = 0;
    if(opendev.isNull() == 1)
    {
        return;
    }
    else
    {
        opendevEle = opendev.toElement();
        attr = opendevEle.attributeNode("bitmap");
        if(!attr.isNull())
        {
            bitMapTemp = attr.value().toInt();
        }
    }
    *bitMapOut = bitMapTemp;
}

void getSceneList(unsigned int roomIdIn, QDomNode & scenesNode,SceneStatusList & listScene)
{
    QDomNode sceneNode;
    //QDomAttr attr;
    QDomElement sceneEle;
    QString strAttrId,strAttrStatus;
    SceneXml scene;

    if(scenesNode.isNull())
    {
        return;
    }

    sceneNode = scenesNode.firstChild();
    while(!sceneNode.isNull())
    {
        if(sceneNode.nodeName()!="scene")
        {
            continue;
        }
        sceneEle  =sceneNode.toElement();
        strAttrId = sceneEle.attribute("id");
        strAttrStatus = sceneEle.attribute("status");
        if(  (!strAttrId.isEmpty())
             &&(!strAttrStatus.isEmpty())
          )
        {
            scene.roomId = roomIdIn;
            scene.sceneId = strAttrId.toInt();
            scene.sceneStatus = strAttrStatus.toInt();
            listScene.push_back(scene);
        }
        sceneNode = sceneNode.nextSibling();
    }
}

void getDevList(unsigned int roomIdIn, int devTypeIn, QDomNode & devsNode,DevList & listDevOut)
{
    if(devsNode.isNull())
    {
        return;
    }

    if(devTypeIn<DEV_TYPE_CURTAIN || devTypeIn>DEV_TYPE_HAVC)
    {
        return;
    }

    QDomNode devNode,charNode;
    QDomElement devEle,charEle;
    QString strDevId;

    QString strChrCid;
    QString strChrVal;

    //QString strDevsName[2] = {"curtains","havcs"};
    QString strDevName[2] = {"curtain","havc"};

    devNode = devsNode.firstChild();
    while(!devNode.isNull())
    {
        if(devNode.nodeName()!=strDevName[devTypeIn])
        {
            continue;
        }
        devEle = devNode.toElement();
        strDevId = devEle.attribute("devId");
        if(strDevId.isEmpty())
        {
            continue;
        }
        charNode = devNode.firstChild();
        while(!charNode.isNull())
        {
            if(charNode.nodeName()!="char")
            {
                continue;
            }
            charEle = charNode.toElement();
            strChrCid = charEle.attribute("cid");
            strChrVal = charEle.attribute("value");
            if(!strChrCid.isEmpty()
                    &&!strChrVal.isEmpty())
            {
                DevsXml stDevInfo;
                stDevInfo.roomId = roomIdIn;
                stDevInfo.devType = devTypeIn;
                stDevInfo.devId = strDevId.toInt();
                stDevInfo.cid = strChrCid.toInt(0,16);
                stDevInfo.val = strChrVal.toInt();
                listDevOut.push_back(stDevInfo);
            }

            charNode = charNode.nextSibling();
        }

        devNode = devNode.nextSibling();
    }
}


void CStatusXml::getAllDevs(DevList& devListOut,LightStatusList& lightListOut,SceneStatusList &listScenesOut)
{
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();

    //QString name = root.nodeName();
    //QDomNodeList listStatus=root.elementsByTagName("status");
    QDomElement status = root;

    if(root.nodeName()!="status")
    {
        return;
    }

    QDomNode rooms = status.namedItem("rooms");
    char bitMapTemp = 0;
    if(rooms.isNull() == 1)
    {
        return;
    }
    QDomNode roomNode = rooms.firstChild();
    QDomElement roomEle;
    QDomAttr attrRoom;
    unsigned int roomId = 0;

    QDomNode devsNode;
    QDomElement devsEle;
    unsigned int lightStatus = 0;
    unsigned int lightDim = 0;
    QDomAttr attrLight;
	QDomAttr attrLightDim;
    while(!roomNode.isNull())
    {
        roomEle = roomNode.toElement();
        attrRoom = roomEle.attributeNode("id");
        if(!attrRoom.isNull())
        {
            roomId = attrRoom.value().toInt();
        }
        devsNode = roomNode.firstChild();
        while(!devsNode.isNull())
        {
            devsEle = devsNode.toElement();
            if(devsNode.nodeName() == "light")
            {
                attrLight = devsEle.attributeNode("status");
                if(!attrLight.isNull())
                {
                    lightStatus = attrLight.value().toInt();
                    LightXml lightNode;
                    lightNode.lightStatus = lightStatus;
                    lightNode.roomId = roomId;
                    lightListOut.push_back(lightNode);
                }

				attrLightDim = devsEle.attributeNode("dim");
                if(!attrLightDim.isNull())
                {
                    lightDim = attrLightDim.value().toInt();
                    LightXml lightDimNode;
                    lightDimNode.lightDim = lightDim;
                    lightDimNode.roomId = roomId;
                    lightListOut.push_back(lightDimNode);
                }
            }
            else if(devsNode.nodeName() == "scenes")
            {
                SceneStatusList listScene1Room;
                getSceneList(roomId, devsNode,listScene1Room);
                if(listScene1Room.count()>0)
                listScenesOut.append(listScene1Room);
            }
            else if(devsNode.nodeName() == "curtains")
            {
                DevList curtainList;
                getDevList(roomId, DEV_TYPE_CURTAIN, devsNode,curtainList);
                devListOut.append(curtainList);
            }
            else if(devsNode.nodeName() == "havcs")
            {
                DevList ListHavc;
                getDevList(roomId, DEV_TYPE_HAVC, devsNode,ListHavc);
                devListOut.append(ListHavc);
            }
            devsNode = devsNode.nextSibling();
        }
        roomNode = roomNode.nextSibling();
    }
}

