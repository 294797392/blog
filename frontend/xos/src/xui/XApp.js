import React from 'react';
import './XApp.css';

class XApp extends React.Component
{
    constructor(props)
    {
        super(props);

        this.onDoubeClickHandler = this.onDoubeClickHandler.bind(this);
    }

    onDoubeClickHandler()
    {
        if(this.props.onOpen)
        {
            this.props.onOpen(this);
        }
    }

    render()
    {
        return (
            <div className="xapp" onDoubleClick={this.onDoubeClickHandler}>
                <div className="xapp-icon">
                    <img className="xapp-img" src={this.props.icon}/>
                </div>
                <div className="xapp-name"></div>
            </div>
        );
    }
}

export default XApp;
