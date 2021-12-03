import React from 'react';
import './App.css';
import XTaskbar from './xui/XTaskbar'
import XDesktop from './xui/XDesktop'
import XLaunchPad from './xui/XLaunchPad'

class App extends React.Component
{
	constructor(props)
	{
		super(props);

		// 禁用右键菜单
		document.oncontextmenu = function() { return false; }
		document.onkeyup = function(e) { console.log(e); }
	}

	componentDidMount()
	{
	}

	render()
	{
		return (
			<div className="xos">
				<XDesktop/>
				<XTaskbar/>
				<XLaunchPad/>
			</div>
		);
	}
}

export default App;
