#pragma once

#include <QtCore/QObject>
#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>

#include "utilsglobal.h"

//! Вызывает функтор в потоке, которому принадлежит объект
/*! Пример:\n
    \code
        QThread t;
        QObject o;
        o.moveToThread(&t);

        postToObject([&]{ o.setObjectName("hello"); }, &o);
        postToObject(std::bind(&QObject::setObjectName, &o, "hello"), &o);
    \endcode
!*/
template <typename F>
UTILSLIB void postToObject(F &&fun, QObject *obj = qApp) {
    QMetaObject::invokeMethod(obj, std::forward<F>(fun));
}


//! Вызывает функтор в текущем или указанном потоке
/*! Пример:\n
    \code
        QThread t;
        QObject o;
        o.moveToThread(&t);

        postToThread([]{ qDebug() << "hello from worker thread"; });
        postToThread([]{ qDebug() << "hello from main thread"; });
    \endcode
!*/
template <typename F>
UTILSLIB void postToThread(F && fun, QThread *thread = qApp->thread()) {
    auto *obj = QAbstractEventDispatcher::instance(thread);
    Q_ASSERT(obj);
    QMetaObject::invokeMethod(obj, std::forward<F>(fun));
}


//! Вызывает метод или слот объекта в потоке, которому принадлежит объект
/*! Пример:\n
    \code
        QThread t;
        struct MyObject : QObject
        {
            void method() {}
        } obj;
        obj.moveToThread(&t);

        postToObject(&obj, &MyObject::method);
    \endcode
!*/
template <typename T, typename R>
UTILSLIB void postToObject(T * obj, R(T::* method)()) {
    struct Event : public QEvent {
        T * obj;
        R(T::* method)();
        Event(T * obj, R(T::*method)()):
            QEvent(QEvent::None), obj(obj), method(method) {}
        ~Event() { (obj->*method)(); }
    };
    QCoreApplication::postEvent(obj, new Event(obj, method));
}
