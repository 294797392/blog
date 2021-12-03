import React from 'react';
import './XDesktop.css';
import XWindow from './XWindow'
import SystemProperties from './../utility/SystemProperties'
import XFileBrowser from './XFileBrowser'

class XDesktop extends React.Component
{
	constructor(props)
	{
		super(props);

		// 禁用右键菜单
		document.oncontextmenu = function(){return false;}
        document.onkeyup = function(e){ console.log(e); }

        this.state = {
            windows : []
        };

        this.renderWindow = this.renderWindow.bind(this);
        this.onXFileIconDoubleClickHandler = this.onXFileIconDoubleClickHandler.bind(this);
    }

    createWindow(w)
    {
        const window = {
            id:1,
            title: w.title,
            width: SystemProperties.XWINDOW_WIDTH,
            height: SystemProperties.XWINDOW_HEIGHT,
            top: SystemProperties.XWINDOW_TOP,
            left: SystemProperties.XWINDOW_LEFT
        };
        let windows = this.state.windows;
        windows.push(window);
        this.setState({windows: windows});
    }

    componentDidMount()
    {
        this.createWindow({title:"1234"});
    }

    onXFileIconDoubleClickHandler(event)
    {
        this.createWindow({title:"new_window"});
    }

    renderWindow(window, index)
    {
        return (
            <XWindow key={index} window={window}>
                <XFileBrowser onXFileIconDoubleClick={this.onXFileIconDoubleClickHandler}/>
            </XWindow>
        )
    }
    
	render()
	{
		return (
			<div className="xdesk">
                {this.state.windows.map(this.renderWindow)}
			</div>
        );
	}
}

export default XDesktop;
