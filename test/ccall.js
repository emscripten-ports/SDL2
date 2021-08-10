let bz=new BroadcastChannel('bez');
document.getElementById('btn').addEventListener('click',function(){
bz.postMessage({data: 222});
});
document.getElementById('di').addEventListener('click',function(){
Module.ccall("pl");
});
