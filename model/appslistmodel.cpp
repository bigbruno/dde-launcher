#include "appslistmodel.h"
#include "global_util/themeappicon.h"
#include "appsmanager.h"

#include <QSize>
#include <QDebug>

AppsListModel::AppsListModel(CategoryID id, QObject *parent) :
    QAbstractListModel(parent),
    m_appsManager(new AppsManager(this))
{
    m_fileInfoInterface = new FileInfoInterface(this);
    m_appCategory = id;

}

int AppsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_appCategory == CategoryID::All) {
    return m_appsManager->appsInfoList().size();
    } else {
        for(int i(0); i < 11; i++) {
            if (m_appCategory == CategoryID(i)) {

        ItemInfoList tmpCateItemInfoList = m_appsManager->getCategoryItemInfo(i);
                return tmpCateItemInfoList.length();
            } else {
                continue;
            }
        }
        return 0;
    }
}

bool AppsListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    // TODO: not support remove multiple rows
    Q_ASSERT(count == 1);

    beginRemoveRows(parent, row, row);
    m_appsManager->removeRow(row);
    endRemoveRows();

    return true;
}

bool AppsListModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data)
    Q_UNUSED(action)
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(parent)

    return true;
}

QVariant AppsListModel::data(const QModelIndex &index, int role) const
{

    if (m_appCategory == CategoryID::All) {
        if (!index.isValid() || index.row() >= m_appsManager->appsInfoList().size())
            return QVariant();
        switch (role)
        {
        case AppNameRole:
        {
            qDebug() << "all appNameRole:" << m_appsManager->appsInfoList()[index.row()].m_name;
            return m_appsManager->appsInfoList()[index.row()].m_name;
        }
        case AppIconRole:
        {
            QString appName =  m_appsManager->appsInfoList()[index.row()].m_iconKey;
            appName = m_fileInfoInterface->GetThemeIcon(m_appsManager->appsInfoList()[index.row()].m_url,
                    64);
            qDebug() << "all iconRole:" << appName;
            return appName;
        }
        case ItemSizeHintRole:
            return QSize(150, 150);

        default:
            return QVariant();
        }
    } else {
        for(int i(0); i < 11; i++) {
            if (m_appCategory == CategoryID(i)) {
                ItemInfoList tmpCateItemInfoList = m_appsManager->getCategoryItemInfo(i);
                qDebug() << "List model:" << i << tmpCateItemInfoList.length();

                if (!index.isValid() || index.row() >= tmpCateItemInfoList.size())
                    return QVariant();
                else
                    qDebug() << "kdkdkdkdkdkd";
                switch (role) {
                case AppNameRole:
                {
                    qDebug() << "appNameRole:" <<  tmpCateItemInfoList.at(index.row()).m_name;
                    qDebug() << tmpCateItemInfoList.at(index.row()).m_name;
                    return  tmpCateItemInfoList.at(index.row()).m_name;
                }
                case AppIconRole:
                {
                    QString appUrl = tmpCateItemInfoList.at(index.row()).m_url;
                    appUrl = m_fileInfoInterface->GetThemeIcon(tmpCateItemInfoList.at(index.row()).m_url,
                            64);
                    qDebug() << "appIconRole:" << appUrl;
                    return appUrl;
                }
                case ItemSizeHintRole:
                    return QSize(150, 150);
                default:
                {
                    qDebug() << "default role";
                    return QVariant();
                }
                }

            } else {
                continue;
            }
        }
        return QVariant();

    }

    return QVariant();
}

void AppsListModel::setListModelData(CategoryID cate) {
    m_appCategory = cate;

}

Qt::ItemFlags AppsListModel::flags(const QModelIndex &index) const
{
//    if (!index.isValid() || index.row() >= m_appsManager->appsInfoList().size())
//        return Qt::NoItemFlags;

    const Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}