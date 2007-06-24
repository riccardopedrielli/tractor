#include <QDateTime>

#include "TShared.h"
#include "md5.h"
#include "TXml.h"

QStringList TShared::updateShareList(QString sharepath, QString sharedlistpath)
{
	QStringList duplicatedid;
	updateShareList(QDir(sharepath),duplicatedid,sharedlistpath);
	return duplicatedid;
}

void TShared::updateShareList(QDir dir, QStringList &duplicatedid, QString sharedlistpath)
{	
	int dim;
	QFileInfoList list;
	dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::Name);
	list = dir.entryInfoList();
	
	dim = list.count();

	for(int i = 0; i < dim; i++)
	{
		dir.cd(list[i].fileName());
		updateShareList(dir,duplicatedid,sharedlistpath);
		dir.cdUp();
	}

	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	list = dir.entryInfoList();
	
	dim = list.count();

	for(int i = 0; i < dim; i++)
	{
		QString name, id, lastmod, complete, path;

		if(!TXml::getFileInfo(sharedlistpath, TAG_PATH, list[i].absoluteFilePath(), 
			name, id, lastmod, complete, path) || lastmod != 
			list[i].lastModified().toString(Qt::ISODate))
		{
			QString fid = createFileId(list[i].absoluteFilePath());
			if(!TXml::getFileInfo(sharedlistpath, TAG_ID, fid, name, id, lastmod,
				complete, path))
			{
				TXml::writeFileInfo(
					sharedlistpath,
					list[i].fileName(),
					fid,
					QString::number(list[i].size()),
					list[i].lastModified().toString(Qt::ISODate),
					QString("yes"),
					list[i].absoluteFilePath()
				);
			}
			else
			{
				duplicatedid.append(list[i].absoluteFilePath());
			}
		}
	}
}

void TShared::checkShareList(QString sharedlistpath)
{
	QStringList pathlist = TXml::getPathList(sharedlistpath);
	QFileInfo file;
	int dim = pathlist.count();

	for (int i = 0; i < dim;i++)
	{
		file = pathlist[i];

		if (!file.exists())
			TXml::deleteFile(sharedlistpath, TAG_PATH, pathlist[i]);		
	}
}

QString TShared::createFileId(QString path)
{
	QFile file;
	QString fid;
	file.setFileName(path);
	file.open(QIODevice::ReadOnly);
	while(!file.atEnd())
	{
		for(int i=0; !file.atEnd() || i<32;i++)
		{
			fid += qtMD5(file.read(4096));
		}

		fid = qtMD5(fid.toAscii());
	}
	file.close();
	return fid;
}
