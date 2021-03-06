#include <QtGui/QApplication>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QIcon>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QToolButton>
#include <QtGui/QLayout>
#include <QtGui/QWheelEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QPushButton>
#include <QtCore/QObjectList>
#include <QtGui/QMessageBox>
#include <QtGui/QFocusEvent>
#include <QtGui/QAction>
#include <QtGui/QDesktopWidget>
#include <QtCore/QDebug>

#include <ZLibrary.h>
#include <ZLPopupData.h>

#include "ZLQtApplicationWindow.h"
#include "../dialogs/ZLQtDialogManager.h"
#include "../view/ZLQtViewWidget.h"
#include "../util/ZLQtKeyUtil.h"

#include "../menu/DrillDownMenu.h"

#include "../dialogs/ScrollerManager.h"

#ifdef 	__SYMBIAN__
#include "../platform/VolumeKeysCapturer.h"
#endif

//#include "../actions/LibraryActions.h"
//TODO remove it:
//#include "../../../../../fbreader/src/fbreader/FBReaderActions.h"

void ZLQtDialogManager::createApplicationWindow(ZLApplication *application) const {
        myApplicationWindow = new ZLQtApplicationWindow(application);
}

ZLQtMenuAction::ZLQtMenuAction(ZLQtApplicationWindow* parent, DrillDownMenuDialog* dialog,  ZLMenubar::PlainItem& item)
    : MenuAction(dialog), myItem(item), myParent(parent) { }

void ZLQtMenuAction::run() {
        //commented, because
        //DrillDownMenuDialog will be close due to parameters of it's constructor
        //myDialog->close();
	myParent->onMenuItemPress(myItem);
}

void ZLQtApplicationWindow::setToggleButtonState(const ZLToolbar::ToggleButtonItem &) { }

ZLQtApplicationWindow::ZLQtApplicationWindow(ZLApplication *application) :
		ZLApplicationWindow(application) {

		const std::string iconFileName = ZLibrary::ImageDirectory() + ZLibrary::FileNameDelimiter + ZLibrary::ApplicationName() + ".png";
		QPixmap icon(QString::fromStdString(iconFileName));
                setWindowIcon(icon);

                ScrollerManager::setScrollerProperties();

                myMenuDialog = new DrillDownMenuDialog(true,this);
		myMenu = new DrillDownMenu;

#ifdef 	__SYMBIAN__
		myVolumeKeyCapture = new VolumeKeysCapturer(this);
#endif
}

void ZLQtApplicationWindow::init() {
		ZLApplicationWindow::init();

                const std::string& mainMenu = ZLDialogManager::Instance().buttonName(ZLResourceKey("menu"));
		myShowMenuAction = new QAction(QString::fromStdString(mainMenu),this);
		connect(myShowMenuAction, SIGNAL(triggered()), this, SLOT(showMenu()));
                addAction(myShowMenuAction);

#ifndef 	__SYMBIAN__
		this->menuBar()->addAction(myShowMenuAction);
#endif

#ifdef __SYMBIAN__
                myShowMenuAction->setSoftKeyRole( QAction::PositiveSoftKey );
#endif

#ifndef 	__SYMBIAN__
		this->menuBar()->addAction(myShowMenuAction);
#endif

                myMenuDialog->showDrillDownMenu(myMenu);

                showFullScreen();
}

void ZLQtApplicationWindow::showMenu() {
        myMenuDialog->setFixedHeight(MenuItemParameters::getMenuDialogHeight());
	myMenuDialog->runNoFullScreen();
}

void ZLQtApplicationWindow::setFullscreen(bool) {
	return;
}

bool ZLQtApplicationWindow::isFullscreen() const {
	return true;
}

void ZLQtApplicationWindow::initMenu() {
	ZLApplicationWindow::initMenu();
}

void ZLQtApplicationWindow::addMenuItem(ZLMenu::ItemPtr item) {
	DrillDownMenuItem* menuItem = 0;
	switch (item->type()) {
			case ZLMenu::Item::ITEM:
				{
					ZLMenubar::PlainItem& plainItem = (ZLMenubar::PlainItem&)*item;
					QString text = QString::fromStdString(plainItem.name());
					menuItem = new DrillDownMenuItem(text, new ZLQtMenuAction(this,myMenuDialog,plainItem) );
					myMenu->addItem(menuItem);
				}
				break;
			case ZLMenu::Item::SUBMENU:
					//TODO make support for submenu
					break;
			case ZLMenu::Item::SEPARATOR:
					//TODO make support for separator
					break;
	}
	if (menuItem != 0) {
			myMenuItems[&*item] = menuItem;
	}
}

void ZLQtApplicationWindow::setMenuItemState(ZLMenu::ItemPtr item, bool visible, bool enabled) {
        Q_UNUSED(enabled);
	DrillDownMenuItem *menuItem = myMenuItems[&*item];
	if (menuItem != 0) {
		menuItem->setHidden(!visible);
	}
}

ZLQtApplicationWindow::~ZLQtApplicationWindow() {
		// delete myVolumeKeyCapture; // TODO fix app crashing here
}

void ZLQtApplicationWindow::keyPressEvent(QKeyEvent *event) {
        application().doActionByKey(ZLQtKeyUtil::keyName(event));
}


void ZLQtApplicationWindow::wheelEvent(QWheelEvent *event) {
        if (event->orientation() == Qt::Vertical) {
                if (event->delta() > 0) {
                        application().doActionByKey(ZLApplication::MouseScrollUpKey);
                } else {
                        application().doActionByKey(ZLApplication::MouseScrollDownKey);
                }
        }
}

void ZLQtApplicationWindow::closeEvent(QCloseEvent *event) {
        if (application().closeView()) {
                event->accept();
        } else {
                event->ignore();
        }
}

void ZLQtApplicationWindow::addToolbarItem(ZLToolbar::ItemPtr) {
	// do nothing; no toolbars
}

ZLQtRunPopupAction::ZLQtRunPopupAction(QObject *parent, shared_ptr<ZLPopupData> data, size_t index) : QAction(parent), myData(data), myIndex(index) {
		setText(QString::fromStdString(myData->text(myIndex)));
        connect(this, SIGNAL(triggered()), this, SLOT(onActivated()));
}

ZLQtRunPopupAction::~ZLQtRunPopupAction() {
}

void ZLQtRunPopupAction::onActivated() {
        myData->run(myIndex);
}

void ZLQtApplicationWindow::setToolbarItemState(ZLToolbar::ItemPtr, bool, bool) { }

void ZLQtApplicationWindow::processAllEvents() {
        qApp->processEvents();
}

ZLViewWidget *ZLQtApplicationWindow::createViewWidget() {
                ZLQtViewWidget *viewWidget = new ZLQtViewWidget(this, &application());
                connect(viewWidget, SIGNAL(tapBottomZoneClicked()), this, SLOT(showMenu()));
		setCentralWidget(viewWidget->widget());
		//viewWidget->widget()->show();
		return viewWidget;
}

void ZLQtApplicationWindow::close() {
        QMainWindow::close();
}

void ZLQtApplicationWindow::grabAllKeys(bool mode) { Q_UNUSED(mode); }

void ZLQtApplicationWindow::setCaption(const std::string &caption) {
		QMainWindow::setWindowTitle(QString::fromStdString(caption));
}

void ZLQtApplicationWindow::setFocusToMainWidget() {
        centralWidget()->setFocus();
}


