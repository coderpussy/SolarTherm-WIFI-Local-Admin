
$( window ).on("load", thermoGraph($(".header .menu li:first-child span").attr("id")));

$(".header .menu li span").click(function() {
    var id = $(this).attr('id');
    thermoGraph(id);

    if ($(".header .menu-btn").is(":checked") == true) {
        $(".header .menu-btn").prop("checked", false);
        $(".header .menu").removeAttr("style");
    }
});

function thermoGraph(thermometer) {
    
    var xhttp = new XMLHttpRequest(); //New request object

    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //This is where you handle what to do with the response 
            var obj = JSON.parse(this.responseText);

            //console.log(obj);

            /*
            I loved Virgil's Dribble shot and decided to remake
            it with some SVG animations :) I didn't really want
            to recreate the curvy line since I believe straight
            lines are more accurate. Sorry Virgil, I might make
            them curvy one day :)! With a bit of time on my 
            hands I could make it even more detailed.
            I hope you like it and post some feedback on how to
            make this better :)!

            Don't forget to check out the original at
            https://goo.gl/0aKiS0

            Kudos Virgil!*/

            var chart_h = 40;
            var chart_w = 80;
            var stepX = 77 / 14;

            /*var chart_1_y = [
            15, 25, 40, 30, 45, 40, 35, 55, 37, 50, 60, 45,70, 78
            ];
            var chart_2_y = [
            80, 65, 65, 40, 55, 34, 54, 50, 60, 64, 55, 27, 24, 30
            ];*/

            var chart_1_y = obj.temphistory;
            var chart_2_y = obj.humhistory;

            function point(x, y) {
                x: 0;
                y: 0;
            }

            /* DRAW GRID */
            function drawGrid(container, graph, minX, maxY, minY) {
                $(graph+" g#grid,"+container+" .valueX li,"+container+" .valueY li").remove();

                var graph = Snap(graph);
                var g = graph.g();
                g.attr('id', 'grid');

                var stepvalY = maxY / Math.round(stepX + 1);

                for (i = 0; i <= stepX + 2; i++) {
                    var horizontalLine = graph.path(
                        "M" + 0 + "," + stepX * i + " " +
                        "L" + 77 + "," + stepX * i);
                    horizontalLine.attr('class', 'horizontal');
                    g.add(horizontalLine);

                    $(container+' .valueY').append("<li>"+Math.round(i*stepvalY)+"</span></li>");
                };
                for (i = 0; i <= 14; i++) {
                    var verticalLine = graph.path(
                        "M" + stepX * i + "," + 38.7 + " " +
                        "L" + stepX * i + "," + 0)
                    verticalLine.attr('class', 'vertical');
                    g.add(verticalLine);
                    
                    $(container+' .valueX').append("<li>"+new Date(new Date().getTime() - (((minX/14)*i) * 60 * 60 * 1000)).getHours()+"</li>");
                };
            }

            drawGrid('#graph-2-container','#chart-2',24,100,0);
            drawGrid('#graph-1-container','#chart-1',24,50,-20);
            /*END DRAW GRID*/

            function drawLineGraph(graph, points, max_y, container, id) {
                $(container+" "+graph+" #graph-"+id+","+container+" "+graph+" #poly-"+id).remove();

                var graph = Snap(graph);

                /* PARSE POINTS */
                var myPoints = [];
                var shadowPoints = [];

                function parseData(points, max_y) {
                    for (i = 0; i < points.length; i++) {
                        var p = new point();
                        var pv = points[i] / max_y * 40;
                        p.x = 83.7 / points.length * i + 1;
                        p.y = 40 - pv;
                        if (p.x > 78) {
                            p.x = 78;
                        }
                        myPoints.push(p);
                    }
                }

                var segments = [];

                function createSegments(p_array) {
                    for (i = 0; i < p_array.length; i++) {
                        if (isNaN(p_array[i].x)) p_array[i].x = p_array[i-1].x;
                        if (isNaN(p_array[i].y)) p_array[i].y = p_array[i-1].y;

                        var seg = "L" + p_array[i].x + "," + p_array[i].y;
                        if (i === 0) {
                            seg = "M" + p_array[i].x + "," + p_array[i].y;
                        }
                        segments.push(seg);
                    }
                }

                function joinLine(segments_array, id) {
                    var line = segments_array.join(" ");
                    var line = graph.path(line);
                    line.attr('id', 'graph-' + id);
                    var lineLength = line.getTotalLength();

                    line.attr({
                        'stroke-dasharray': lineLength,
                        'stroke-dashoffset': lineLength
                    });
                }

                function calculatePercentage(points, graph) {
                    var initValue = points[0];
                    var endValue = points[points.length - 1];
                    var sum = endValue - initValue;
                    var prefix;
                    var percentageGain;
                    var stepCount = 1300 / sum;

                    function findPrefix() {
                        if (sum > 0) {
                            prefix = "+";
                        } else {
                            prefix = "";
                        }
                    }

                    var percentagePrefix = "";

                    function percentageChange() {
                        percentageGain = initValue / endValue * 100;
                        
                        if(percentageGain > 100){
                            console.log('over100');
                            percentageGain = Math.round(percentageGain * 100*10) / 100;
                        }else if(percentageGain < 100){
                            console.log('under100');
                            percentageGain = Math.round(percentageGain * 10) / 10;
                        }
                        if (initValue > endValue) {
                        
                            percentageGain = endValue/initValue*100-100;
                            percentageGain = percentageGain.toFixed(2);
                        
                            percentagePrefix = "";
                            $(graph).find('.percentage-value').addClass('negative');
                        } else {
                            percentagePrefix = "+";
                        }
                        if(endValue > initValue){
                            percentageGain = endValue/initValue*100;
                            percentageGain = Math.round(percentageGain);
                        }
                    };

                    percentageChange();
                    findPrefix();

                    var percentage = $(graph).find('.percentage-value');
                    var totalGain = $(graph).find('.total-gain');
                    var hVal = $(graph).find('.h-value');

                    function count(graph, sum) {
                        var totalGain = $(graph).find('.total-gain');
                        var i = 0;
                        var time = 1300;
                        var intervalTime = Math.abs(time / sum);
                        var timerID = 0;
                        if (sum > 0) {
                            var timerID = setInterval(function () {
                                i++;
                                totalGain.text(percentagePrefix + i);
                                if (i === sum) clearInterval(timerID);
                            }, intervalTime);
                        } else if (sum < 0) {
                            var timerID = setInterval(function () {
                                i--;
                                totalGain.text(percentagePrefix + i);
                                if (i === sum) clearInterval(timerID);
                            }, intervalTime);
                        }
                    }
                    count(graph, sum);

                    percentage.text(percentagePrefix + percentageGain + "%");
                    totalGain.text("0%");
                    setTimeout(function () {
                        percentage.addClass('visible');
                        hVal.addClass('visible');
                    }, 1300);

                }

                function showValues() {
                    var val1 = $(graph).find('.h-value');
                    var val2 = $(graph).find('.percentage-value');
                    val1.addClass('visible');
                    val2.addClass('visible');
                }

                function drawPolygon(segments, id) {
                    var lastel = segments[segments.length - 1];
                    var polySeg = segments.slice();
                    polySeg.push([78, 38.4], [1, 38.4]);
                    var polyLine = polySeg.join(' ').toString();
                    var replacedString = polyLine.replace(/L/g, '').replace(/M/g, "");

                    var poly = graph.polygon(replacedString);
                    var clip = graph.rect(-80, 0, 80, 40);
                    poly.attr({
                        'id': 'poly-' + id,
                        /*'clipPath':'url(#clip)'*/
                        'clipPath': clip
                    });
                    clip.animate({
                        transform: 't80,0'
                    }, 1300, mina.linear);
                }

                parseData(points, max_y);
                
                createSegments(myPoints);
                //calculatePercentage(points, container);
                joinLine(segments,id);
            
                drawPolygon(segments, id);
                
                /*$('#poly-'+id).attr('class','show');*/

                /* function drawPolygon(segments,id){
                var polySeg = segments;
                polySeg.push([80,40],[0,40]);
                var polyLine = segments.join(' ').toString();
                var replacedString = polyLine.replace(/L/g,'').replace(/M/g,"");
                var poly = graph.polygon(replacedString);
                poly.attr('id','poly-'+id)
                }
                drawPolygon(segments,id);*/
            }
            function drawCircle(container,id,progress,unit,parent){
                $( parent+" "+container+" #circle-graph-"+id ).remove();

                var paper = Snap(container);
                var prog = paper.path("M5,50 A45,45,0 1 1 95,50 A45,45,0 1 1 5,50");
                var lineL = prog.getTotalLength();
                var oneUnit = lineL/100;
                var toOffset = lineL - oneUnit * progress;
                var myID = 'circle-graph-'+id;
                prog.attr({
                    'stroke-dashoffset':lineL,
                    'stroke-dasharray':lineL,
                    'id':myID
                });
                
                var animTime = 1300; /*progress / 100*/
                
                prog.animate({
                    'stroke-dashoffset':toOffset
                },animTime,mina.easein);
                
                function countCircle(animtime,parent,unit,progress){
                    var textContainer = $(parent+' .circle-percentage').text(progress);
                    var time = 1300;

                    $(textContainer).each(function (index) {
                        $(this).prop('Counter',0).animate({
                            Counter: $(this).text()
                        }, {
                            duration: time,
                            easing: 'swing',
                            step: function (now) {
                                $(this).text(parseFloat(now).toFixed(1)+unit);
                            }
                        });
                    });

                    /*var textContainer = $(parent).find('.circle-percentage');
                    var i = 0;
                    var time = 1300;
                    var intervalTime = Math.abs(time / progress);
                    var timerID = setInterval(function () {
                        i++;
                        textContainer.text(i+unit);
                        if (i === progress) clearInterval(timerID);
                    }, intervalTime);*/
                }
                countCircle(animTime,parent,unit,progress);
            }
            
            function showFirmwareVersion(devicename) {
                var xmlhttp = new XMLHttpRequest();
                xmlhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        $('#firmware').text(this.responseText);
                    }
                };
                xmlhttp.open("GET", "../get-firmware.php?device=" + devicename, true);
                xmlhttp.send();
            }

            $('.thermtitle').text(obj.name);

            const event = new Date(obj.lastupdate);
            const options = { weekday: 'short', year: 'numeric', month: 'short', day: 'numeric' };
            $('.lastupdate').text(event.toLocaleDateString('de-DE', options) + ' ' + event.toLocaleTimeString('de-DE'));

            var sensor3 = $('.sensor-3').find('.data-3').text(obj.pressure + 'hPa');
            var sensor4 = $('.sensor-4').find('.data-4').text(obj.altitude + 'm');
            var sensor5 = $('.sensor-5').find('.data-5').text(obj.battery + ' Volt');

            setTimeout(function () {
                sensor3.addClass('visible');
                sensor4.addClass('visible');
                sensor5.addClass('visible');
            }, 1300);

            drawCircle('#chart-3',1,parseFloat(obj.temperature),'°C','#circle-1');
            drawCircle('#chart-4',2,parseFloat(obj.humidity),'%','#circle-2');
            drawLineGraph('#chart-1', chart_1_y, 50, '#graph-1-container', 1);
            drawLineGraph('#chart-2', chart_2_y, 100, '#graph-2-container', 2);

            showFirmwareVersion(obj.name.toLowerCase());
        }
    }; 
    xhttp.open("get", "get-data.php?device="+thermometer, true); 
    // ^ Don't block the rest of the execution. 
    // Don't wait until the request finishes to // continue. 
    xhttp.send();
}