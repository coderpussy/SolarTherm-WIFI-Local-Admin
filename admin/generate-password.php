<?php
// Handle AJAX request (start)
if( isset($_POST['ajax']) && isset($_POST['pepper']) ){
    echo $hash = password_hash($_POST['pepper'], PASSWORD_DEFAULT);
 exit;
}
// Handle AJAX request (end)
?>

<!doctype html>
    <html>

    <body >
        <h1>SolarTherm API key password hash generator</h1>
        <h2>For SolarTherm WIFI Local Admin project</h2>
        <p>Don't forget to enter the password salt in <code>remote-temerature.php</code> to <code>$password = 'S47TnP3pp3R';</code> for your database writing.</p>
        <p>And the generated password hash to <code>config.h</code> in your Arduino sketch to <code>LOCAL_STORAGE_API_KEY</code></p>
        <p><code>'S47TnP3pp3R'</code> should a self defined password string.</p>

        <form id="passgen" method='post' action>
            <input type='text' name='pepper' placeholder='Enter password salt string' id='pepper'>
            <input type='submit' value='Generate' name='submit'><br><br>
            <div id='response'></div>
        </form>

        <!-- Script -->
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
        <script>
        $(document).ready(function(){
            $('#passgen').submit(function(ev){
                var pepper = $('#pepper').val();
                
                ev.preventDefault();
                $.ajax({
                    type: 'post',
                    data: {ajax: 1,pepper: pepper},
                    success: function(response){
                        $('#response').text('Password hash: ' + response);
                    }
                });
            });
        });
        </script>
    </body>
</html>