/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtAddOn.JsonDb module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef JSONDBMODELUTILS_H
#define JSONDBMODELUTILS_H
#include <QSharedData>
#include <QStringList>
#include <QUuid>
#include <QJSValue>
#include <QVariant>
#include <QPointer>
#include <QJsonDbWatcher>
#include <QJsonDbReadRequest>

QT_BEGIN_NAMESPACE_JSONDB

struct CallbackInfo {
    int index;
    QJSValue successCallback;
    QJSValue errorCallback;
};

struct NotifyItem {
    int partitionIndex;
    QVariantMap item;
    QJsonDbWatcher::Action action;
};

struct SortIndexSpec
{
    QString propertyName;
    QString propertyType; //### TODO remove
    QString name;
    bool caseSensitive;
    enum Type { None, String, Number, UUID };
    Type type;

    SortIndexSpec() : caseSensitive(false), type(SortIndexSpec::None) {}
    SortIndexSpec(const SortIndexSpec &other)
        : propertyName(other.propertyName),
          propertyType(other.propertyType),
          name(other.name),
          caseSensitive(other.caseSensitive),
          type(other.type)
    {}

};
class JsonDbListModelPrivate;
class ModelRequest : public QObject
{
    friend class JsonDbListModelPrivate;
    Q_OBJECT
public:

    ModelRequest(QObject *parent = 0);
    ~ModelRequest();

    QJsonDbReadRequest* newRequest(int newIndex);
    void resetRequest();
Q_SIGNALS:
    void finished(int index, const QList<QJsonObject> &items, const QString &sortKey);
    void error(QtJsonDb::QJsonDbRequest::ErrorCode code, const QString &message);

private Q_SLOTS:
    void onQueryFinished();

private:
    QPointer<QJsonDbReadRequest> request;
    int index;
};

struct IndexInfo
{
    SortIndexSpec spec;
    bool valid;
    int requestId;
    IndexInfo () {clear();}
    void clear() {
        requestId = -1;
        valid = false;
        spec.type = SortIndexSpec::None;
    }
};

class SortingKeyPrivate;

class SortingKey {
public:
    SortingKey(int partitionIndex, const QVariantMap &object, const QList<bool> &directions, const QList<QStringList> &paths, const SortIndexSpec &spec = SortIndexSpec());
    SortingKey(int partitionIndex, const QVariantList &object, const QList<bool> &directions, const SortIndexSpec &spec = SortIndexSpec());
    SortingKey(int partitionIndex, const QByteArray &uuid, const QVariantList &object, const QList<bool> &directions, const SortIndexSpec &spec = SortIndexSpec());
    SortingKey(const SortingKey&);
    SortingKey() {}
    int partitionIndex() const;
    bool operator <(const SortingKey &rhs) const;
    bool operator ==(const SortingKey &rhs) const;
private:
    QSharedDataPointer<SortingKeyPrivate> d;
};

struct RequestInfo
{
    int lastOffset;
    int lastSize;
    int requestCount;
    QPointer<QJsonDbWatcher> watcher;

    RequestInfo() { clear();}
    void clear()
    {
        lastOffset = 0;
        lastSize = -1;
        requestCount = 0;
        if (watcher) {
            delete watcher;
            watcher = 0;
        }
    }
};

class SortingKeyPrivate : public QSharedData {
public:
    SortingKeyPrivate(int index, const QByteArray &objectUuid, QList<bool> objectDirections, QVariantList objectKeys, const SortIndexSpec &spec)
        :uuid(objectUuid), directions(0), values(0), count(0), partitionIndex(index), indexSpec(spec)
    {
        count = objectDirections.count();
        directions = new bool[count];
        for (int i=0; i<count; i++) {
            directions[i] = objectDirections[i];
        }
        count = objectKeys.count();
        values = new QVariant[count];
        for (int i=0; i<count; i++) {
            values[i] = objectKeys[i];
        }
    }
    ~SortingKeyPrivate()
    {
        delete [] directions;
        delete [] values;
    }
    QByteArray uuid;
    bool *directions;
    QVariant *values;
    int count;
    int partitionIndex;
    SortIndexSpec indexSpec;
};

template <typename T> int iterator_position(T &begin, T &end, T &value)
{
    int i = 0;
    for (T itr = begin;(itr != end && itr != value); i++, itr++) {}
    return i;
}

QVariant lookupProperty(QVariantMap object, const QStringList &path);
QString removeArrayOperator(QString propertyName);
QList<QJsonObject> qvariantlist_to_qjsonobject_list(const QVariantList &list);
QVariantList qjsonobject_list_to_qvariantlist(const QList<QJsonObject> &list);

QT_END_NAMESPACE_JSONDB

#endif // JSONDBMODELUTILS_H
