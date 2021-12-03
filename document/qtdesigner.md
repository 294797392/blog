# QTDesigner介绍

今天研究了一下午的QTDesigner的用法，突然发现QTDesigner非常强大，可以让我们开发QT程序像开发Winform程序一样方便，界面上的所有Weidget都可以使用QTDesigner来设计，而不用去手写C++代码（因为我目前接手的项目是使用QT开发的，但是所有的UI布局全部都是手写C++代码去布局，这样特别不方便，而且要写非常多的重复的代码，感觉开发起来异常痛苦，所以特意研究了下QTDesigner），
如果把QTDesigner研究好，那么你可以在任意平台（包括但不限于Android，IOS，OSX，Windows）去很方便的做GUI程序。

QT还有一个特别强大的地方就是可以使用css来对Widget设置样式。。这非常方便

QT使用一个专门的编译器-uic，来把后缀名为ui的文件编译成C++代码，这点其实是和Winform是类似的。ui文件本质就是一个xml文件，里面保存了界面上所有的Widget和属性，可以使用QTDesigner直接打开ui文件来进行各种Widget的拖拽，属性配置，甚至可以把你自己开发的Widget集成到QTDesigner中当成一个普通的Widget去使用。

在使用QT VS插件的时候，可以直接在VS里新建ui文件，在编译项目的时候，插件会自动调用uic去把ui文件编译成C++布局类，在你自己的类里可以使用ui这个变量去访问所有的Widget，这样其实也很方便，但是每次访问Widget都需要用ui这个变量，感觉很不爽。
在使用Winform开发的时候，因为用的是C#语言，C#语言有个“部分类”的特性，使得一个类可以写多个实现，Winform使用了一个部分类来专门对UI进行布局，C++里没有“部分类”的概念，这里我使用了C++多重继承的特性，让我们自己的Widget首先继承QT的Widget，然后继承uic生成的布局文件，这样每次访问Widget的时候，就不用通过ui这个变量来访问了。

# QTDesigner使用技巧





