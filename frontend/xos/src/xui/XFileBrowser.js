import React from 'react';
import SystemProperties from './../utility/SystemProperties'
import './XFileBrowser.css'
import ImageFile from './../images/xfile-browser-file.png'

class XFileGroupHeader extends React.Component
{
    constructor(props)
    {
        super(props);
    }

    render()
    {
        return (
            <div className="xfile-group-header">
                <div className="xfile-group-header-name">分类1</div>
                <div className="xfile-group-header-line"></div>
            </div>
        );
    }
}

class XFileIcon extends React.Component
{
    constructor(props)
    {
        super(props);

        this.onDoubleClickHandler = this.onDoubleClickHandler.bind(this);
        this.onClickHandler = this.onClickHandler.bind(this);
    }

    onDoubleClickHandler()
    {
        if(this.props.onDoubleClick)
        {
            this.props.onDoubleClick(this.props.file);
        }
    }

    onClickHandler()
    {
        if(this.props.onClick)
        {
            this.props.onClick(this.props.file);
        }
    }

    render()
    {
        return (
            <div className={this.props.selected ? "xfile-icon xfile-icon-selected" : "xfile-icon"} onDoubleClick={this.onDoubleClickHandler} onClick={this.onClickHandler}>
                    <div className="xfile-icon-icon">
                        <img className="xfile-icon-img" src={ImageFile}/>
                    </div>
                    <div className="xfile-icon-name">{this.props.file.name}</div>
            </div>
        );
    }
}

class XFileList extends React.Component
{
    constructor(props)
    {
        super(props);

        this.state = {
            "selectedFileID": 0
        };

        this.onXFileIconClickHandler = this.onXFileIconClickHandler.bind(this);
        this.onXFileIconDoubleClickHandler = this.onXFileIconDoubleClickHandler.bind(this);
        this.renderXFileIcon = this.renderXFileIcon.bind(this);
    }

    onXFileIconClickHandler(file)
    {
        const changed = this.state.selectedFileID != file.id;
        if(changed)
        {
            this.setState({
                "selectedFileID": file.id
            });

            if(this.props.onSelectionChanged)
            {
                this.props.onSelectionChanged(file);
            }
        }
    }

    onXFileIconDoubleClickHandler(file)
    {
        if(this.props.onDoubleClick)
        {
            this.props.onDoubleClick(file);
        }
    }

    renderXFileIcon(file, index)
    {
        return (
            <XFileIcon key={file.id} file={file} selected={file.id == this.state.selectedFileID} onClick={this.onXFileIconClickHandler} onDoubleClick={this.onXFileIconDoubleClickHandler}/>
        )
    }

    render()
    {
        return (
            <div className="xfile-list">
                {
                    this.props.files.map(this.renderXFileIcon)
                }
            </div>
        );
    }
}

class XFileGroup extends React.Component
{
    constructor(props)
    {
        super(props);

        this.onSelectionChangedHandler = this.onSelectionChangedHandler.bind(this);
        this.onDoubleClickHandler = this.onDoubleClickHandler.bind(this);
    }

    onSelectionChangedHandler(file)
    {
    }

    onDoubleClickHandler(file)
    {
        if(this.props.onDoubleClick)
        {
            this.props.onDoubleClick({"file":file});
        }
    }

    render()
    {
        const files = [
            {id:1,name:"文件"},
            {id:2,name:"文件"},
            {id:3,name:"文件"},
            {id:4,name:"文件"},
            {id:5,name:"文件"},
            {id:6,name:"文件"},
            {id:7,name:"文件"},
            {id:8,name:"文件"},
            {id:9,name:"文件"},
            {id:10,name:"文件"},
            {id:11,name:"文件"},
            {id:12,name:"文件"},
        ];

        return (
            <div className="xfile-group">
                <XFileGroupHeader/>
                <XFileList files={files} onSelectionChanged={this.onSelectionChangedHandler} onDoubleClick={this.onDoubleClickHandler}/>
            </div>
        );
    }
}

class XFileBrowser extends React.Component
{
	constructor(props)
	{
        super(props);

        this.onXFileGroupDoubleClickHandler = this.onXFileGroupDoubleClickHandler.bind(this);
    }

    componentDidMount()
    {
    }

    onXFileGroupDoubleClickHandler(event)
    {
        if(this.props.onXFileIconDoubleClick)
        {
            this.props.onXFileIconDoubleClick({"file":event.file});
        }
    }

	render()
	{
		return (
			<div className="xfile-browser">
                <XFileGroup onDoubleClick={this.onXFileGroupDoubleClickHandler}/>
                <XFileGroup/>
                <XFileGroup/>
                <XFileGroup/>
                <XFileGroup/>
                <XFileGroup/>
                <XFileGroup/>
                <XFileGroup/>
			</div>
		);
	}
}

export default XFileBrowser;
