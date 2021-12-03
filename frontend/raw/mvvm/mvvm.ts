enum BindingMode
{
    TwoWay = 0,
    OneWay = 1
}

interface Binding
{

}

interface INotifyPropertyChanged
{
}

export default class MVVM 
{
//#region 实例变量

    private bindingList: Array<Binding>;

//#endregion

    private constructor()
    {
        this.bindingList = new Array<Binding>();
    }

    static getInstance()
    {
        return this;
    }

    public bind(source: Object, target: HTMLElement, path: string, mode: BindingMode)
    {
        let notify = source as INotifyPropertyChanged;
        if(notify != null)
        {
        }
    }
};
