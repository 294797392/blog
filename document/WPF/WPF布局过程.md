## Panel布局过程

WPF的布局过程分三个步骤，分别是：
1.  Measure：确定子元素的大小
2.  Arrange：排列子元素
3.  Render：渲染界面

## Measure过程
https://learn.microsoft.com/zh-cn/dotnet/desktop/wpf/advanced/layout?view=netframeworkdesktop-4.8#LayoutSystem_Measure_Arrange
https://www.codeproject.com/Articles/1034445/Understanding-MeasureOverride-and-ArrangeOverride

MeasureOverride用来对Panel里的每个子元素进行大小的测量。

MeasureOverride的输入参数是FrameworkElement所测量到的可用空间大小，输出参数是Panel所需要的实际空间大小。
当FrameworkElement测量出来的可用空间比MeasureSize大的时候，使用可用空间大小去进行布局（Arrange）和渲染（Render）
当Panel的大小动态改变（比如动态设置宽高，或者动态调整窗口大小）导致大小比MeasureSize小的时候，使用MeasureSize去进行布局和渲染，并且在Panel里会显示一个滚动条
可以用一行代码来概括WPF是如何决定布局大小和渲染大小的：
finalSize = Math.Max(MeasureSize, FrameworkElement所测量到的可用空间大小);

1. UIElement -> Measure（公开接口，用户可以调用）
Clip和Visibility会影响到UIElement阶段的大小测量

2. FramworkElement -> MeasureCore（由UIElement -> Measure调用）
Height，Width，Margin，Style会影响FrameworkElement阶段的大小测量

3. Panel -> MeasureOverride（由FrameworkElement -> MeasureCore调用。用户可以通过重写此方法来影响布局系统）

4. 重写MeasureOverride方法
* 在MeasureOverride方法里需要对每个子元素进行大小测量（调用每个子元素的Measure方法），然后通过所有子元素的大小来计算自己所需要的实际空间大小，最后返回给WPF布局系统。
* 在重写MeasureOverride方法的时候，需要考虑子元素和Panel的Margin等一系列有可能会影响到空间大小的值。
* 对子元素调用Measure方法之后，会将测量后的大小保存到子元素的DesiredSize属性里，在Arrange阶段使用子元素的DesiredSize进行布局。
* 如果没有对子元素调用Measure方法，那么子元素的DesiredSize将始终为0。
* 关于MeasureSize返回值：
当可用空间比MeasureSize大的时候，使用可用空间大小去进行布局（Arrange）和渲染（Render）；当可用空间动态改变（比如动态设置宽高，或者动态调整窗口大小）为比MeasureSize小的时候，使用MeasureSize去进行布局和渲染，并且在Panel里会显示一个滚动条
可以用一行代码来概括WPF是如何决定布局大小和渲染大小的：
finalSize = Math.Max(MeasureSize, FrameworkElement所测量到的可用空间大小);


## Arrange过程

ArrangeOverride输入参数：finalSize = Math.Max(MeasureSize, FrameworkElement所测量到的可用空间大小);
ArrangeOverride输出参数：RenderSize

Arrange用来确定子元素在Panel里的位置。

1. 重写ArrangeOverride方法
在ArrangeOverride方法里需要计算每个子元素的位置，计算好位置后，对每个子元素调用Arrange方法并把计算好的位置传给Arrange方法。
ArrangeOverride方法返回最终的显示大小，也就是RenderSize，一般情况下ArrangeOverride可以不去修改finalSize参数，可以直接返回finalSize参数。

## Render过程

WPF使用ArrangeOverride返回的finalSize去渲染界面

## DesiredSize和RenderSize

DesireSize是控件所占空间大小，包括了Margin等布局属性
RenderSize是显示控件的实际大小，不包含Margin等属性的大小，渲染是按照RenderSize来渲染的
