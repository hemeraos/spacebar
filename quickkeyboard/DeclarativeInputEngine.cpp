//============================================================================
/// \file   DeclarativeInputEngine.cpp
/// \author Uwe Kindler
/// \date   08.01.2015
/// \brief  Implementation of CDeclarativeInputEngine
///
/// Copyright 2015 Uwe Kindler
/// Licensed under MIT see LICENSE.MIT in project root
//============================================================================

#include "DeclarativeInputEngine.h"

#include <QInputMethodEvent>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtQml>
#include <QTimer>
#include <QtGui/QKeyEvent>

/**
 * Private data class
 */
struct DeclarativeInputEnginePrivate
{
    DeclarativeInputEngine* _this;
    bool Animating;
    QTimer* AnimatingFinishedTimer;//< triggers position adjustment of focused QML item is covered by keybaord rectangle
    int InputMode;
    QRect KeyboardRectangle;
    QObject *focusObject;

    /**
     * Private data constructor
     */
    DeclarativeInputEnginePrivate(DeclarativeInputEngine* _public);
}; // struct DeclarativeInputEnginePrivate


DeclarativeInputEnginePrivate::DeclarativeInputEnginePrivate(DeclarativeInputEngine* _public)
    : _this(_public),
      Animating(false),
      InputMode(DeclarativeInputEngine::Latin)
{

}


DeclarativeInputEngine::DeclarativeInputEngine(QObject *parent) :
    QObject(parent),
    d(new DeclarativeInputEnginePrivate(this))
{
    d->focusObject = NULL;

    d->AnimatingFinishedTimer = new QTimer(this);
    d->AnimatingFinishedTimer->setSingleShot(true);
    d->AnimatingFinishedTimer->setInterval(100);
    connect(d->AnimatingFinishedTimer, SIGNAL(timeout()), this, SLOT(animatingFinished()));
}


DeclarativeInputEngine::~DeclarativeInputEngine()
{
    delete d;
}


void DeclarativeInputEngine::virtualKeyCancel()
{

}

bool DeclarativeInputEngine::virtualKeyClick(Qt::Key key, const QString & text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(key)
    Q_UNUSED(modifiers)

    if (Q_UNLIKELY(!d->focusObject)) {
        qDebug() << "No focus object\n";
        return false;
    }

    if (text == QStringLiteral("\x7F")) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::postEvent(d->focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::postEvent(d->focusObject, releaseEvent);

    } else if (text == QStringLiteral("\n")) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(d->focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(d->focusObject, releaseEvent);

    } else {
        QInputMethodEvent ev;
        ev.setCommitString(text);
        QCoreApplication::sendEvent(d->focusObject, &ev);
    }

    return true;
}

void DeclarativeInputEngine::sendKeyToFocusItem(const QString& text)
{
    if (Q_UNLIKELY(!d->focusObject)) {
        qDebug() << "No focus object\n";
        return;
    }

    if (text == QStringLiteral("\x7F")) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::postEvent(d->focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::postEvent(d->focusObject, releaseEvent);

    } else if (text == QStringLiteral("\n")) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(d->focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(d->focusObject, releaseEvent);

    } else {
        QInputMethodEvent ev;
        ev.setCommitString(text);
        QCoreApplication::sendEvent(d->focusObject, &ev);
    }
}

bool DeclarativeInputEngine::virtualKeyPress(Qt::Key key, const QString & text, Qt::KeyboardModifiers modifiers, bool repeat)
{
    Q_UNUSED(key)
    Q_UNUSED(text)
    Q_UNUSED(modifiers)
    Q_UNUSED(repeat)

    // not implemented yet
    return true;
}

bool DeclarativeInputEngine::virtualKeyRelease(Qt::Key key, const QString & text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(key)
    Q_UNUSED(text)
    Q_UNUSED(modifiers)

    // not implemented yet
    return true;
}

QRect DeclarativeInputEngine::keyboardRectangle() const
{
    return d->KeyboardRectangle;
}

void DeclarativeInputEngine::setKeyboardRectangle(const QRect& Rect)
{
    setAnimating(true);
    d->AnimatingFinishedTimer->start(100);
    d->KeyboardRectangle = Rect;
    emit keyboardRectangleChanged();
}

bool DeclarativeInputEngine::isAnimating() const
{
    return d->Animating;
}

void DeclarativeInputEngine::setAnimating(bool Animating)
{
    if (d->Animating != Animating)
    {
        d->Animating = Animating;
        emit animatingChanged();
    }
}

void DeclarativeInputEngine::animatingFinished()
{
    setAnimating(false);
}

int DeclarativeInputEngine::inputMode() const
{
    return d->InputMode;
}

void DeclarativeInputEngine::setInputMode(int Mode)
{
    if (Mode != d->InputMode)
    {
        d->InputMode = Mode;
        emit inputModeChanged();
    }
}

void DeclarativeInputEngine::setFocusObject(QObject *object)
{
    d->focusObject =  object;
}
