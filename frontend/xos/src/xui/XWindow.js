import React from 'react';
import './XWindow.css';

class XWindow extends React.Component
{
    m_dragStartPosX = 0;
    m_dragStartPosY = 0;
    m_startPosX = 0;
    m_startPoxY = 0;

    constructor(props)
    {
        super(props);

        this.state = {
            title: this.props.window.title,
            width: this.props.window.width,
            height: this.props.window.height,
            top: this.props.window.top,
            left: this.props.window.left
        };

        this.handleDragStart = this.handleDragStart.bind(this);
        this.handleDrag = this.handleDrag.bind(this);
        this.handleDragEnd = this.handleDragEnd.bind(this);
        this.handleDoubleClick = this.handleDoubleClick.bind(this);
    }

    handleDragStart(e)
    {
        this.m_dragStartPosX = e.clientX;
        this.m_dragStartPosY = e.clientY;
        this.m_startPosX = parseInt(this.state.left);
        this.m_startPoxY = parseInt(this.state.top);
    }

    handleDrag(e)
    {
        let offsetX = e.clientX - this.m_dragStartPosX;
        let offsetY = e.clientY - this.m_dragStartPosY;
        // console.log(offsetX + "," + offsetY);
        this.setState({
            top: this.m_startPoxY + offsetY,
            left: this.m_startPosX + offsetX
        });
    }

    handleDragEnd(e)
    {
        // console.log("dragover");
        this.handleDrag(e);
    }

    handleDoubleClick(e)
    {
        // console.log("double click");
        // if(this.onDoubleClick)
        // {
        //     this.onDoubleClick(this, e);
        // }
    }

    render()
    {
        return (
            <div className="xwindow" style={{"width": this.state.width, "height": this.state.height, "top": this.state.top, "left": this.state.left}}>
                <div className="xwin-background"></div>
                <div className="xwin-title" draggable="true" onDragStart={this.handleDragStart} onDrag={this.handleDrag} onDragEnd={this.handleDragEnd} onDoubleClick={this.handleDoubleClick}>
                    <div className="xwin-title-widgets">
                        <div className="xwin-title-icon"></div>
                        <div className="xwin-title-name">{this.state.title}</div>
                        <div className="xwin-title-minimum"></div>
                        <div className="xwin-title-maximum"></div>
                        <div className="xwin-title-close"></div>
                    </div>
                </div>
                <div className="xwin-content">
                    <div className="xwin-content-main">
                        {this.props.children}
                    </div>
                </div>
                <div className="xwin-status-bar"></div>
            </div>
        )
    }
}

export default XWindow;
