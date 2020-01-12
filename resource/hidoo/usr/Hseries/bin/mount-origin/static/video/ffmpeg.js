function VideoModule(ip,port,numRow,numCol,video_width,video_height)
{
	// 为了尽可能保证成功率:如果ip为空或还回地址则改用document.domain获取的地址
	if (null == ip || ip == 'undefined' || ip == undefined || "" == ip || "0.0.0.0" == ip || ip.indexOf('127.') == 0) {
		this._videoIP = document.domain; // S1暂使用此方式获取'视频服务器IP'
	} else {
		this._videoIP = ip;
	}

	if (null == port || port == 'undefined' || port == undefined || "" == port || 0 == port) {
		this._videoPort = 8081;
	} else {
		this._videoPort = port;
	}
    this._frame_w = video_width;
    this._frame_h = video_height;
    this._num_w = numCol;
    this._num_h = numRow;

    this.onLoaded = null;
    this.registerCanvasSimple = function(canvas,videoIndex){
        this.registerCanvas(canvas,videoIndex%this._num_w,Math.floor(videoIndex/this._num_h),1,1)
    }
    this.clearCanvas = function()
    {
        while(this._canvasList.length)
        {
            var c = this._canvasList.shift()
            c.canvas = null
            c = null
        }
    }
        
    this.registerCanvas = function(canvas,x_pos,y_pos,x_num_blocks,y_num_blocks){
        var item = {'canvas':canvas,'x_pos':x_pos,'y_pos':y_pos,'x_num_blocks':x_num_blocks,'y_num_blocks':y_num_blocks}
        this._canvasList.push(item)
    };
    this._afterLoaded = function(){
        if(this.onLoaded)
            this.onLoaded(this)
    };
    this._img = null; 
    this._timerWasm = null;
    this._canvasList = [];
    this._clampedBuffer = null;

    this.__drawCanvasOptimize4img = function(){
		var pad = 2//剔除边缘2像素
        if(this._img.Valid != true){
            //VideoModule._img.src = VideoModule._img.src
            return
        }
        for(var i = 0;i < this._canvasList.length;i++)
        {

            var canvas=this._canvasList[i].canvas;
            var x_p_b = this._frame_w/this._num_w
            var y_p_b = this._frame_h/this._num_h
            var x = this._canvasList[i].x_pos*x_p_b
            var y = this._canvasList[i].y_pos*y_p_b
            var w = this._canvasList[i].x_num_blocks*x_p_b
            var h = this._canvasList[i].y_num_blocks*y_p_b
            x = x+pad
            y = y+pad
            w = w - 2*pad
            h = h - 2*pad
            canvasCtx=canvas.getContext("2d");
            try{
                canvasCtx.drawImage(this._img,x,y,w,h,0,0,canvas.width,canvas.height)
            }
            catch(err){
                this._img.Valid = false
                //VideoModule._img.src = VideoModule._img.src
            }
        }
		
    };
		
    this._canvasBuffer = null;
    this._canvasImgDataBuffer = null;
    
    //start init
    this._img =  document.createElement("img");
    this._img.vm = this
    this._img.Idle = true
    this._img.Valid = false
    this._img.onload = function(){
	    this.Idle = true
	    this.Valid = true
        URL.revokeObjectURL(this.src);
//        if(this.width == 1920){
            this.vm.__drawCanvasOptimize4img()
//        }
    }

	this._canvasBuffer =  document.createElement("canvas");
	this._canvasBuffer.width = this._frame_w
	this._canvasBuffer.height = this._frame_h
	this._canvasImgDataBuffer = this._canvasBuffer.getContext("2d").getImageData(0,0,this._canvasBuffer.width,this._canvasBuffer.height);

	var decodeWorker = new Worker("/static/video/decode_worker.js")
    decodeWorker.vm = this
	decodeWorker.onmessage = function (event){
		if(event.data.type == 2){
			var jpgData = event.data.frame.slice(4,event.data.frame.length)
            var url = URL.createObjectURL(new Blob([jpgData]));
            if(this.vm._img.Idle == true){
                this.vm._img.src = url;
                this.vm._img.Idle = false;
            }
            url=null;
            jpgData = null;
		}
        event.data.frame=null;
        event.data.type=null;
        event.data=null;
        event=null;
	}
    var data = {type:3,width:this._frame_w,height:this._frame_h}
    decodeWorker.postMessage(data)
	var wsUrl = 'ws://'+this._videoIP+':'+this._videoPort;
    var ws = new WebSocket(wsUrl);
    ws.binaryType="arraybuffer"
	ws.onmessage = function(evt){
        var u8Res = new Uint8Array(evt.data);
        var data = {type:1,h264Buff:u8Res};
        decodeWorker.postMessage(data);
        data=null;
        u8Res=null;
        evt.data=null;
        evt=null;
	}
}

