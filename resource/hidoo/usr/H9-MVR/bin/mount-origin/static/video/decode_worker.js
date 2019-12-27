
importScripts('/static/video/libffmpeg.js');

var cxt = null
var start = false
var frameObjList = []
var w = 0;
var h = 0;
function waitWasmLoading()
{
    console.log("timer ...")
    if('_zx_createH264Decoder' in Module.asm && w != 0 && h != 0)
    {

        console.log("timer stop");
        clearInterval(timerWasm);
        setTimeout("start = true;",1000);
        cxt = Module._zx_createH264Decoder(w,h)
        postMessage({type:1})
    }
}
var timerWasm = setInterval(waitWasmLoading,100)
function copyData2(ptr,len)
{
    return Module.HEAPU8.slice(ptr,ptr+len)
}
function copyData(ptr,len)
{

    var buff = new ArrayBuffer(len)
    var u8Buff = new Uint8Array(buff)
    for(var i = 0;i < len;i++)
        u8Buff[i] = Module._zx_getMemVal(ptr,i)
    return u8Buff
}
onmessage = function (event){
    if(event.data.type == 1) //recv h264
    {
        if(start == false || cxt == null)
            return
        var h264Buff = event.data.h264Buff
        var buffer = Module._malloc(h264Buff.length)
        Module.HEAPU8.set(h264Buff, buffer);
        Module._zx_decode(cxt,buffer,h264Buff.length)
	Module._free(buffer)
	
        var frame =_zx_getFrame(cxt);
        while(frame != 0)
        {
			var len = Module._zx_getMemVal(frame)
			len += Module._zx_getMemVal(frame+1)<<8
			len += Module._zx_getMemVal(frame+2)<<16
			len += Module._zx_getMemVal(frame+3)<<24
            frameBuff = copyData2(frame,len+4)
            Module._free(frame)
           
            var data = {type:2,frame:frameBuff}
            postMessage(data)
            frame =_zx_getFrame(cxt);
        }
    }
    else if(event.data.type == 3)
    {
        w = event.data.width;
        h = event.data.height;
    }
    event.data.type=null;
    event.data.h264Buff=null;
    event.data=null;
    evemt=null;

}

