# QWidget show函数不显示窗口
如果QWidget有父元素，那么QWidgte默认作为一个部件，而不是一个窗口，它只能显示在父部件里，没法直接以窗口的形式显示。但是你可以使用setWindowFlag(Qt::Window)函数来让QWidget强制作为一个窗口显示，不管窗口有没有获得焦点，都会永远显示在父窗口之上。
如果QWidget没有父元素，那么QWidget必须作为一个窗口，并且以窗口的形式显示。