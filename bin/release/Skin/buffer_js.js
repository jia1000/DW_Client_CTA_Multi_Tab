 var currFilePath;
        var currFilePaths;
        var read_begin_index = 0;
        var read_begin_index_sync = 0;

        var request_type_text = "";
        var image_operation_text = "";
        var image_paras_text = "";

        var array_files_datas = new Array();

        /// 点击“发送字符串到C++”后， c++端返回命令，用来显示发送的次数
        function jsSendCount(text) {
            var t = document.getElementById("recv_text");
            t.value = text;
        }

         function loadFileFromLoacalUseBuffer() {
             var file = currFilePaths[read_begin_index];
             // read file content.
             var reader = new FileReader();
             reader.readAsArrayBuffer(file);
             // alert("loadFileFromLoacal");
             reader.onload = function(evt) {
                 // 保证只保存n个文件
                 var file = evt.target.result; // 读取文件内容
                 array_files_datas[read_begin_index_sync] = file;
                 // console.log(read_begin_index_sync);
                 // alert("sync: " + read_begin_index_sync);
                 read_begin_index_sync++;
                 if(read_begin_index_sync >= currFilePaths.length) {
                     // alert("post : " + read_begin_index_sync);
                     read_begin_index = 0;
                     // 重置全局的文件索引
                     writeTime("#middletime", " - middletime");
                     postOneFileUseBuffer();
                 }
             }
         }
        function loadFileFromLoacal() {
            var file = currFilePaths[read_begin_index];
            // read file content.
            var reader = new FileReader();
            reader.readAsArrayBuffer(file);
            // alert("loadFileFromLoacal");
            reader.onload = function(evt) {
                // 保证只保存n个文件
                var file = evt.target.result; // 读取文件内容
                array_files_datas[read_begin_index_sync] = arraybuffer2base64(file);
                // console.log(read_begin_index_sync);
                // alert("sync: " + read_begin_index_sync);
                read_begin_index_sync++;
                if(read_begin_index_sync >= currFilePaths.length) {
                    // alert("post : " + read_begin_index_sync);
                    read_begin_index = 0;
                    // 重置全局的文件索引
                    writeTime("#middletime", " - middletime");
                    postOneFileUseJson();
                }
            }
        }

        function postOneFileUseJson() {
            if(read_begin_index >= currFilePaths.length) {
                return ;
            }

            var json_data = {};
            json_data.request_type      = request_type_text;
            json_data.image_operation   = image_operation_text;
            json_data.image_paras       = image_paras_text;
            json_data.image_data        = array_files_datas[read_begin_index];
            json_data.image_sequence    = read_begin_index;//read_begin_index.toString(10);
            json_data.image_max         = currFilePaths.length;

            var json_str = JSON.stringify(json_data);
            var url = 'http://image_controller/';

            var xhr = new XMLHttpRequest();
            //  第3个参数async为true，表示为异步，否则为同步
            xhr.open('POST', url, true);
            xhr.responseType = "text";//"arraybuffer";

            // 请求成功回调函数
            xhr.onload = function (oEvent) {
                var arrayBuffer = xhr.response;
                if (arrayBuffer) {
                }
                // console.log("send finished.")
                writeTime("#endtime", " - endtime");
                read_begin_index++;
                postOneFileUseJson();
            };
            xhr.send(json_str);
            console.log("send...");
        }
         function postOneFileUseBuffer() {
             if(read_begin_index >= currFilePaths.length) {
                 return ;
             }

             var url = 'http://image_buffer_transfer/';

             var xhr = new XMLHttpRequest();
             //  第3个参数async为true，表示为异步，否则为同步
             xhr.open('POST', url, true);
             xhr.responseType = "text";//"arraybuffer";

             // 请求成功回调函数
             xhr.onload = function (oEvent) {
                 var arrayBuffer = xhr.response;
                 if (arrayBuffer) {
                 }
                 // console.log("send finished.")
                 writeTime("#endtime", " - endtime");
                 read_begin_index++;
                 postOneFileUseBuffer();
             };
             xhr.send(array_files_datas[read_begin_index]);
             console.log("send...");
         }
        ///发送字符串到C++ 使用json
        function doPostMultiFilesUesJson() {
            clearTime("#begintime",     "begintime : ");
            clearTime("#middletime",   "middletime : ");
            clearTime("#endtime",       "endtime   : ");
            writeTime("#begintime", " - begintime");

            request_type_text       = $("#request_type").val();
            image_operation_text    = $("#image_operation").val();
            image_paras_text         = $("#image_paras").val();

            read_begin_index_sync = 0;

            //  读取本地文件到内存
            for(read_begin_index = 0; read_begin_index < currFilePaths.length; read_begin_index++) {
                loadFileFromLoacal();
            }

            // for(read_begin_index = 0; this.read_begin_index < this.currFilePaths.length; read_begin_index++){
            //     // alert("file : " + currFilePaths[read_begin_index].name);
            //     postOneFileUseJson();
            // }
        }

         ///发送字符串到C++ 使用json
         function doPostMultiFilesUesBuffer() {
             clearTime("#begintime",     "begintime : ");
             clearTime("#middletime",   "middletime : ");
             clearTime("#endtime",       "endtime   : ");
             writeTime("#begintime", " - begintime");

             request_type_text       = $("#request_type").val();
             image_operation_text    = $("#image_operation").val();
             image_paras_text         = $("#image_paras").val();

             read_begin_index_sync = 0;

             //  读取本地文件到内存
             for(read_begin_index = 0; read_begin_index < currFilePaths.length; read_begin_index++) {
                 loadFileFromLoacalUseBuffer();
             }
         }
        ///发送字符串到C++ 使用json
        function doPostStringUesJson() {
            clearTime("#begintime",     "begintime : ");
            clearTime("#middletime",   "middletime : ");
            clearTime("#endtime",       "endtime   : ");

            writeTime("#begintime", " - begintime");

            //缓存的file对象
            if (currFilePath === undefined) return;

            // 读取图像文件
            var reader = new FileReader();//新建一个FileReader
            reader.readAsArrayBuffer(currFilePath);//读取文件
            reader.onload = function (evt) { //读取完文件之后会回来这里
                writeTime("#middletime", " - middletime");

                var fileString = evt.target.result; // 读取文件内容

                var text1 = $("#request_type").val();
                var text2 = $("#image_operation").val();
                var text3 = $("#image_paras").val();

                var json_data = {};
                json_data.request_type = text1;
                json_data.image_operation = text2;
                json_data.image_paras = text3;
                // json_data.image_data = arraybuffer2base64(fileString);
                // json_data.file_index = 4;

                var json_str = JSON.stringify(json_data);
                var url = 'http://image_controller/';

                var xhr = new XMLHttpRequest();
                //  第3个参数async为true，表示为异步，否则为同步
                xhr.open('POST', url, true);
                xhr.responseType = "text";//"arraybuffer";

                // 请求成功回调函数
                xhr.onload = function (oEvent) {
                    var arrayBuffer = xhr.response;
                    if (arrayBuffer) {
                        displayContent(xhr.response, true);

                        writeTime("#endtime", " - endtime");
                    }
                };
                xhr.ontimeout = function (e) {
                    alert("ontimeout");
                };
                xhr.onerror = function (e) {
                    alert("onerror-status : " + xhr.status + "status.text: " + xhr.statusText + "....");
                };
                xhr.onloadend = function (e) {
                };

                xhr.send(json_str);
            }
        }

         function doMprPostStringUesJson() {
             clearTime("#begintime",     "begintime : ");
             clearTime("#middletime",   "middletime : ");
             clearTime("#endtime",       "endtime   : ");

             writeTime("#begintime", " - begintime");

             var text1 = "mpr";
             var text2 = $("#mpr_image_operation").val();
             var text3 = $("#mpr_image_paras").val();

             doRequestImageHandle(text1, text2, text3);
        }

         function doCprPostStringUesJson() {
             clearTime("#begintime",     "begintime : ");
             clearTime("#middletime",   "middletime : ");
             clearTime("#endtime",       "endtime   : ");

             writeTime("#begintime", " - begintime");

             var text1 = "cpr";
             var text2 = $("#cpr_image_operation").val();
             var text3 = $("#cpr_image_paras").val();

             doRequestImageHandle(text1, text2, text3);
         }
         function doVrPostStringUesJson() {
             clearTime("#begintime",     "begintime : ");
             clearTime("#middletime",   "middletime : ");
             clearTime("#endtime",       "endtime   : ");

             writeTime("#begintime", " - begintime");

             var text1 = "vr";
             var text2 = $("#vr_image_operation").val();
             var text3 = $("#vr_image_paras").val();

             doRequestImageHandle(text1, text2, text3);
         }
        function doRequestImageHandle(request_type, image_operation, image_paras) {
            var json_data = {};
            json_data.request_type      = request_type;
            json_data.image_operation   = image_operation;
            json_data.image_paras       = image_paras;
            // json_data.image_data = arraybuffer2base64(fileString);
            // json_data.file_index = 4;

            var json_str = JSON.stringify(json_data);
            var url = 'http://image_controller/';

            var xhr = new XMLHttpRequest();
            //  第3个参数async为true，表示为异步，否则为同步
            xhr.open('POST', url, true);
            xhr.responseType = "text";//"arraybuffer";

            // 请求成功回调函数
            xhr.onload = function (oEvent) {
                var arrayBuffer = xhr.response;
                if (arrayBuffer) {
                    displayContent(xhr.response, true);

                    writeTime("#endtime", " - endtime");
                }
            };
            xhr.send(json_str);
        }
        ///显示图像和信息
        function displayContent(text, finished) {
            // 解析json数据
            var json_data = JSON.parse(text);
            var text1 = json_data.request_type;
            var text2 = json_data.image_operation;
            var text3 = json_data.image_paras;
            var text4 = json_data.image_data;

            // 显示传回的参数
            var t = document.getElementById("recv_text");
            var text_paras = "";
            text_paras += "request_type    : " + text1;
            text_paras += "\n";
            text_paras += "image_operation : " + text2;
            text_paras += "\n";
            text_paras += "image_paras     : " + text3;
            text_paras += "\n";
            t.value = text_paras;

            // display the image
            showImageByArrayBuffer(text1, text4);
        }

        // 通过arraybuffer显示图像
        function showImageByArrayBuffer(request_type, data) {
            if(request_type === "mpr") {
                // display the image
                $('#imgtag_mpr').attr("src", "data:image/png;base64," + data);
            } else if(request_type === "vr") {
                // display the image
                $('#imgtag_vr').attr("src", "data:image/png;base64," + data);
            } else if(request_type === "cpr") {
                // display the image
                $('#imgtag_cpr').attr("src", "data:image/png;base64," + data);
            } else if(request_type === "mip") {
                // display the image
                $('#imgtag_mip').attr("src", "data:image/png;base64," + data);
            }
        }
        // arraybuffer 转为base64
        function arraybuffer2base64(data) {
            var bytes = new Uint8Array(data);
            var ret = "";
            var len = bytes.byteLength;
            for (var i = 0; i < len; ++i) {
                ret += String.fromCharCode(bytes[i]);
            }
            return window.btoa(ret);
        }
        /// 获取当前选择的文件
        function jsReadFiles(files) {
            if (files.length) {
                var file = files[0];
                //缓存文件路径
                currFilePath = files[0];
            }
            read_begin_index = 0;
            currFilePaths = files;
            // alert("file count: " + currFilePaths.length);
            // for(var i = 0; i < currFilePaths.length ; i++) {
            //     alert("file name: " + currFilePaths[i].name);
            // }
        }


        // ///写时间标签的函数
        // function clearTime(tagid) {
        //     $(tagid).html("");
        // }
        ///写时间标签的函数
        function clearTime(tagid, timestr) {
            $(tagid).html(timestr);
        }
        ///写时间标签的函数
        function writeTime(tagid, timestr) {
            var myDate      = new Date();
            var timestring  = myDate.toLocaleString();
            var milisec     = myDate.getMilliseconds();
            $(tagid).html(timestring + "." + milisec + " : " + timestr);
        }
