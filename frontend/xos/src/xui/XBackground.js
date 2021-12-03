import React from 'react';
import './XBackground.css';

class XBackground extends React.Component
{
    constructor(props)
    {
        super(props);

        this.state = {
            "backgroundSize": document.documentElement.clientWidth + "px " + document.documentElement.clientHeight + "px"
        };

        this.resizeHandler = this.resizeHandler.bind(this);
    }

    componentDidMount()
	{
        window.addEventListener('resize', this.resizeHandler);
    }

    componentWillUnmount()
    {
        window.removeEventListener('resize', this.resizeHandler);
    }

    resizeHandler(e)
    {
        this.setState({
            "backgroundSize": document.documentElement.clientWidth + "px " + document.documentElement.clientHeight + "px"
        });
    }

    render()
    {
        return (
            <React.Fragment>
                <div className="xbackground1" style={{"backgroundSize":this.state.backgroundSize}}></div>
                <div className="xbackground2"></div>
            </React.Fragment>
        )
    }
}

export default XBackground;
