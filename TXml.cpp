#include <QDomText>
#include <QTextStream>
#include <QDomDocument>
#include <QStringList>

#include "TXml.h"

void TXml::writeFileInfo(QString sharedpath, QString name, QString id, QString size,
			QString lastmod, QString complete, QString path)
{
	const int Indent = 4;
	QDomDocument doc;
	QDomElement dshared = doc.createElement("shared"),
				dfile = doc.createElement("file"),
				dname = doc.createElement("name"),
				did = doc.createElement("id"),
				dsize = doc.createElement("size"),
				dlastmod = doc.createElement("lastmod"),
				dcomplete = doc.createElement("complete"),
				dpath = doc.createElement("path");

	QDomText filename = doc.createTextNode(name),
			 fileid = doc.createTextNode(id),
			 filesize = doc.createTextNode(size),
			 filelastmod = doc.createTextNode(lastmod),
			 filecomplete = doc.createTextNode(complete),
		     filepath = doc.createTextNode(path);

	dfile.appendChild(dname);
	dfile.appendChild(did);
	dfile.appendChild(dsize);
	dfile.appendChild(dlastmod);
	dfile.appendChild(dcomplete);
	dfile.appendChild(dpath);
	dname.appendChild(filename);
	did.appendChild(fileid);
	dsize.appendChild(filesize);
	dlastmod.appendChild(filelastmod);
	dcomplete.appendChild(filecomplete);
	dpath.appendChild(filepath);
	
	QFile file(sharedpath);
	file.open(QIODevice::ReadWrite | QIODevice::Text);

	if(!file.size())
	{
		doc.appendChild(dshared);
		dshared.appendChild(dfile);	
	}
	else
	{
		QString errorStr;
		int errorLine;
		int errorColumn;		
		if (!doc.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) 
		{
			qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
								 .arg(errorLine)
					             .arg(errorColumn)
						         .arg(errorStr).toAscii());
			file.close();
			return;
		}
		QDomElement root = doc.documentElement();
		
		if (root.tagName() != "shared")
		{
			file.close();
			return;
		}
		
		root.appendChild(dfile);
	}
	
	file.reset();
	QTextStream out(&file);
	doc.save(out, Indent);
	file.close();	
}

bool TXml::getFileInfo(QString xmlpath, InfoTagName tagname, QString value, 
		QString &name, QString &id, QString &lastmod, QString &complete, QString &path)
{	
	QString errorStr;
	int errorLine;
    int errorColumn;
    QDomDocument doc;
	QFile file(xmlpath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	if(!file.size())
	{
		file.close();
		return false;
	}
    
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
        qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
							 .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr).toAscii());
		file.close();
        return false;
    }

    QDomElement root = doc.documentElement();

	if (root.tagName() != "shared")
		return false;

    QDomNode node = root.firstChild();

    while (!node.isNull()) 
	{
		QDomNodeList nodelist = node.childNodes();	
	
       	if ( nodelist.at(tagname).toElement().text() == value)
		{			
			name = nodelist.at(TAG_NAME).toElement().text();
			id = nodelist.at(TAG_ID).toElement().text();
			lastmod = nodelist.at(TAG_LASTMOD).toElement().text();
			complete = nodelist.at(TAG_COMPLETE).toElement().text();
			path = nodelist.at(TAG_PATH).toElement().text();
			file.close();
			return true;
		}

        node = node.nextSibling();
    }
	file.close();
	return false;
}

bool TXml::setFileInfo(QString xmlpath, InfoTagName tagname, QString value, 
					   InfoTagName settagname, QString setvalue)
					   
{
	const int Indent = 4;
	QString errorStr;
	int errorLine;
    int errorColumn;
    QDomDocument doc;
	QFile file(xmlpath);
	file.open(QIODevice::ReadWrite | QIODevice::Text);
	
	if(!file.size())
	{
		file.close();
		return false;
	}
    
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
        qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
							 .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr).toAscii());
		file.close();
        return false;
    }

    QDomElement root = doc.documentElement();

	if (root.tagName() != "shared")
	{
		file.close();
		return false;
	}

    QDomNode node = root.firstChild();

    while (!node.isNull()) 
	{
		QDomNodeList nodelist = node.childNodes();	
	
       	if ( nodelist.at(tagname).toElement().text() == value)
		{			
			QDomNode n = nodelist.at(settagname).toElement().firstChild();
			n.toText().setData(setvalue);	
			file.reset();
			QTextStream out(&file);
			doc.save(out, Indent);
			file.close();
			return true;
		}

        node = node.nextSibling();
    }
	file.close();
	return false;
}

QStringList TXml::getPathList(QString xmlpath)
{
	QStringList list;
	QString errorStr;
	int errorLine;
    int errorColumn;
    QDomDocument doc;
	QFile file(xmlpath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	if(!file.size())
	{
		file.close();
		return list;
	}
    
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
        qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
							 .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr).toAscii());
		file.close();
        return list;
    }

    QDomElement root = doc.documentElement();

	if (root.tagName() != "shared")
	{
		file.close();
		return list;
	}

    QDomNode node = root.firstChild();

    while (!node.isNull()) 
	{
		QDomNodeList nodelist = node.childNodes();	       	
		list.append(nodelist.at(TAG_PATH).toElement().text());
        node = node.nextSibling();
    }
	file.close();
	return list;
}

bool TXml::deleteFile(QString xmlpath, InfoTagName tagname, QString value)
{
	const int Indent = 4;
	QString errorStr;
	int errorLine;
    int errorColumn;
    QDomDocument doc;
	QFile file(xmlpath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	if(!file.size())
	{
		file.close();
		return false;
	}
    
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
	{
        qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
							 .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr).toAscii());
		file.close();
        return false;
    }

    QDomElement root = doc.documentElement();

	if (root.tagName() != "shared")
		return false;

    QDomNode node = root.firstChild();

    while (!node.isNull()) 
	{
		QDomNodeList nodelist = node.childNodes();	
	
       	if ( nodelist.at(tagname).toElement().text() == value)
		{			
			root.removeChild(node);
			file.close();
			file.open(QIODevice::WriteOnly | QIODevice::Text);
			QTextStream out(&file);
			doc.save(out, Indent);
			file.close();
			return true;
		}

        node = node.nextSibling();
    }
	file.close();
	return false;
}

QList<FileInfo> TXml::getFileList(QString xmlpath)
{
	QList<FileInfo> list;
	QString errorStr;
	FileInfo fileinfo;
	int errorLine;
    int errorColumn;
    QDomDocument doc;
	QFile file(xmlpath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	if(!file.size())
	{
		file.close();
		return list;
	}
    
	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
        qDebug(QObject::tr("Parse error at line %1, column %2:\n%3")
							 .arg(errorLine)
                             .arg(errorColumn)
                             .arg(errorStr).toAscii());
		file.close();
        return list;
    }

    QDomElement root = doc.documentElement();

	if (root.tagName() != "shared")
	{
		file.close();
		return list;
	}

    QDomNode node = root.firstChild();

    while (!node.isNull()) 
	{
		QDomNodeList nodelist = node.childNodes();	       	

		fileinfo.name=nodelist.at(TAG_NAME).toElement().text();
		fileinfo.dim=nodelist.at(TAG_SIZE).toElement().text();
		fileinfo.sources="";
		fileinfo.completes=nodelist.at(TAG_COMPLETE).toElement().text();
		fileinfo.fid=nodelist.at(TAG_ID).toElement().text();

		list.append(fileinfo);
        node = node.nextSibling();
    }
	file.close();
	return list;
}
