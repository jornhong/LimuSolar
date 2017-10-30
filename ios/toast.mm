#include <QApplication>
#include <QDebug>
#include "toast.h"
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIView+Toast.h"


void makeIOSToast(const QString &msg)
{
        UIWindow *window = [UIApplication sharedApplication].keyWindow;
        UIViewController *rootViewController = window.rootViewController;
        [rootViewController.view makeToast:msg.toNSString()
            duration:2.0
                  position:CSToastPositionBottom];
}

