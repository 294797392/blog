## 布局过程

WPF的布局过程分三个步骤，分别是：
1.  Measure：确定子元素的大小
2.  Arrange：排列子元素
3.  Render：渲染界面

## Measure过程
https://learn.microsoft.com/zh-cn/dotnet/desktop/wpf/advanced/layout?view=netframeworkdesktop-4.8#LayoutSystem_Measure_Arrange
1. UIElement -> Measure（公开接口，用户可以调用）
Clip和Visibility会影响到UIElement阶段的大小测量

2. FramworkElement -> MeasureCore（由UIElement -> Measure调用）
Height，Width，Margin，Style会影响FrameworkElement阶段的大小测量

3. Panel -> MeasureOverride（由FrameworkElement -> MeasureCore调用。用户可以通过重写此方法来影响布局系统）
Panel在Measure阶段需要对每个子元素进行大小测量（需要在MeasureOverride里调用每个子元素的Measure方法），然后通过所有子元素的大小来计算自己所需要的实际空间大小，最后返回给WPF布局系统。
注意只有Panel才可以重写MeasureOverride方法，其他的控件无法重写此方法，因为Panel的作用是排列子元素。

## Arrange过程


## Render过程


## 自定义布局
通过重写MeasureOverride和ArrangeOverride这两个函数来实现自定义布局

1. MeasureOverride
参数：可用的空间大小
返回值：你的自定义布局占用的空间大小

布局系统首先会调用MeasureOverride，来计算你的自定义布局需要占用多少空间，
在MeasureOverride这个步骤，一般都要遍历控件里的所有的子控件并调用子控件的Measure函数，一旦调用了子控件的Measure函数，子控件就会根据XAML里设置的宽度和高度以及一些其他的属性（比如Margin）去计算一共要占用多少空间，最后你要根据所有的子控件所占空间来确定你的自定义布局所需空间大小。
MeasureOverride执行完之后，WPF会把控件的DesiredSize（期望的大小）设置成MeasureOverride返回的大小，以便在ArrangeOverride的时候可以拿到子控件所占用的空间大小。
如果不去调用子控件的Measure函数，那么就永远不知道子控件需要占用多少空间（DesireSize为NaN），那么就永远没法对子控件进行布局，因为不知道要放到哪里，所以一定要调用子控件的Measure函数。

2. ArrangeOverride
参数：WPF计算出来的你的自定义布局可用的空间
返回值：你的自定义布局实际要渲染的空间大小，WPF会在你返回的这个空间内渲染你的自定义布局（OnRender函数渲染的就是这块区域，比如在这个区域内渲染背景颜色）；注意，如果子元素所占空间大于布局所占空间，那么子元素会超出布局空间范围

最后布局系统会调用ArrangeOverride，通知控件对子元素进行布局，在ArrangeOverride这个步骤，一般都要遍历子控件并调用子控件的Arrange函数，根据子控件的DesiredSize去确定把子控件放在什么地方。
WPF会根据你调用Arrange函数的时候传进去的Rect来渲染子元素，如果你传进去的Rect比子元素的DesireSize小，那么WPF只会显示子控件的Rect区域。
ArrangeOverride函数的参数是WPF测量出来的你的布局可以使用的空间大小；通常情况下，你的布局大小不能超过finalSize，如果超过了，程序并不会出错，但是你的布局会占用到其他控件的布局，从而形成一个重叠区域。
ArrangeOverride函数返回该布局的最终大小，并把RenderSize（渲染大小）设置成ArrangeOverride返回的大小，WPF会根据你返回的大小去渲染界面。
在重写OnRender函数的时候，可以使用RenderSize来渲染界面。


## DesiredSize和RenderSize

DesireSize是控件所占空间大小，包括了Margin等布局属性
RenderSize是显示控件的实际大小，不包含Margin等属性的大小，渲染是按照RenderSize来渲染的
