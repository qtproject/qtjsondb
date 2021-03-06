/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtAddOn.JsonDb module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QJSONDB_REQUEST_P_H
#define QJSONDB_REQUEST_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtJsonDb API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QJsonObject>

#include "qjsondbrequest.h"

QT_BEGIN_NAMESPACE_JSONDB

class QJsonPrivatePartition;
class QJsonDbRequestPrivate
{
    Q_DECLARE_PUBLIC(QJsonDbRequest)
    friend class QJsonPrivatePartition;

public:
    QJsonDbRequestPrivate(QJsonDbRequest *q);
    virtual ~QJsonDbRequestPrivate() { }

    virtual QJsonObject getRequest() const = 0;
    virtual void handleResponse(const QJsonObject &) = 0;
    virtual void handleError(int, const QString &) = 0;

    void setStatus(QJsonDbRequest::Status newStatus);
    void setRequestId(int id);

    bool isPrivatePartition() const;

    void _q_privatePartitionResults(const QList<QJsonObject> &res);
    void _q_privatePartitionStatus(QtJsonDb::QJsonDbRequest::Status status);

    QJsonDbRequest *q_ptr;
    QString partition;
    QJsonDbRequest::Status status;
    int requestId;
    bool internal; // marks internal requests e.g. notification and token auth.
    QList<QJsonObject> results;
};

#define JSONDB_CHECK_REQUEST_STATUS \
    if (d->status >= QJsonDbRequest::Queued) \
        qWarning("QJsonDbRequest: should not change already queued request.");

QT_END_NAMESPACE_JSONDB

#endif // QJSONDB_REQUEST_P_H
