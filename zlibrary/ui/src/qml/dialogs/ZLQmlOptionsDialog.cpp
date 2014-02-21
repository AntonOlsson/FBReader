/*
 * Copyright (C) 2004-2011 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <QEventLoop>
#include <QCoreApplication>

#include <ZLDialogManager.h>
#include <QDebug>
#include <QPointer>

#include "ZLQmlOptionsDialog.h"
#include "ZLQmlDialogContent.h"
#include "ZLQtUtil.h"

ZLQmlOptionsDialog::ZLQmlOptionsDialog(const ZLResource &resource,
                                       shared_ptr<ZLRunnable> applyAction, bool showApplyButton)
    : ZLOptionsDialog(resource, applyAction) {
	myResult = false;
	myOkButtonText = ::qtButtonName(ZLDialogManager::OK_BUTTON);
	if (showApplyButton)
		myApplyButtonText = ::qtButtonName(ZLDialogManager::APPLY_BUTTON);
	myCancelButtonText = ::qtButtonName(ZLDialogManager::CANCEL_BUTTON);
    myTitle = QString::fromStdString(resource[ZLDialogManager::DIALOG_TITLE].value());
}

ZLQmlOptionsDialog::~ZLQmlOptionsDialog() {
    qDebug() << Q_FUNC_INFO << __LINE__;
	mySections.clear();
	emit sectionsChanged(sections());
}

ZLDialogContent &ZLQmlOptionsDialog::createTab(const ZLResourceKey &key) {
	ZLQmlDialogContent *tab = new ZLQmlDialogContent(tabResource(key));
    mySections << QWeakPointer<QObject>(tab);
	myTabs.push_back(tab);
	emit sectionsChanged(sections());
	return *tab;
}

QList<QObject*> ZLQmlOptionsDialog::sections() const {
    QList<QObject*> result;
    foreach (const QWeakPointer<QObject> &obj, mySections) {
		if (!obj.isNull())
			result << obj.data();
	}
	return result;
}

// We don't surely provide tabs at mobile devices
const std::string &ZLQmlOptionsDialog::selectedTabKey() const {
	return myEmptyString;
}

void ZLQmlOptionsDialog::selectTab(const ZLResourceKey &key) {
	Q_UNUSED(key);
}

bool ZLQmlOptionsDialog::runInternal() {
    QEventLoop eventLoop;
	connect(this, SIGNAL(finished()), &eventLoop, SLOT(quit()), Qt::QueuedConnection);
    eventLoop.exec(QEventLoop::AllEvents);
	qApp->sendPostedEvents(0, QEvent::DeferredDelete);
	return myResult;
}

void ZLQmlOptionsDialog::accept() {
	myResult = true;
//	ZLOptionsDialog::accept();
	emit finished();
}

void ZLQmlOptionsDialog::reject() {
	myResult = false;
	emit finished();
}

QString ZLQmlOptionsDialog::okButtonText() const {
	return myOkButtonText;
}

QString ZLQmlOptionsDialog::applyButtonText() const {
	return myApplyButtonText;
}

QString ZLQmlOptionsDialog::cancelButtonText() const {
	return myCancelButtonText;
}

QString ZLQmlOptionsDialog::title() const {
    return myTitle;
}
