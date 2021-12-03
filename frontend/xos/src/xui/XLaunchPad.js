import React from 'react';
import './XLaunchPad.css';
import XBackground from './XBackground';
import XApp from './XApp';
import ImageDefaultApp from './../images/default_app.jpg';

const taotalScalingApp = 7;

class XLaunchPad extends React.Component
{
    constructor(props)
    {
        super(props);

        this.state = {
            "apps":[{},{}]
        };

        this.onOpenAppHandler = this.onOpenAppHandler.bind(this);
        this.onXAppMouseMoveHandler = this.onXAppMouseMoveHandler.bind(this);
        this.renderXApp = this.renderXApp.bind(this);
    }

    componentDidMount()
	{
    }

    componentWillUnmount()
    {
    }

    onOpenAppHandler(xevent)
    {
        if(this.props.onOpenApp)
        {
            this.props.onOpenApp(xevent);
        }
    }

    onXAppMouseMoveHandler(e)
    {
    }

    renderXApp(xapp, index)
    {
        return (
            <div key={index} className="xlpad-app" onMouseMove={this.onXAppMouseMoveHandler} index={index}>
                <XApp icon={ImageDefaultApp} onOpen={this.onOpenAppHandler}/>
            </div>
        )
    }

    render()
    {
        // const apps = [{},{},{},{},{},{},{},{},{}];
        // const apps = [{}];
        // const apps = [{},{}];

        return (
            <div className="xlpad">
                <XBackground/>
                <div className="xlpad-apps-panel">
                    {
                        this.state.apps.map(this.renderXApp)
                    }
                </div>
            </div>
        )
    }
}

export default XLaunchPad;
