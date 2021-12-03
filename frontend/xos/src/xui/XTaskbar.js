import React from 'react';
import './XTaskbar.css';

class XTaskbar extends React.Component
{
    constructor(props)
    {
        super(props);
    }

    render()
    {
        return (
            <div className="xtaskbar">
                <div className="xtaskbar-app-menu"></div>
                <div className="xtaskbar-sys-info"></div>
            </div>
        )
    }
}

export default XTaskbar;
