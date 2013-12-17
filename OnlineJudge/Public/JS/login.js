function LoginBean(username,password){
	this.username=username;
	this.password=password;
}

function RegistBean(username,password,nickname,email){
	this.username=username;
	this.password=password;
	this.nickname=nickname;
	this.email=email;
}


$(function() {	
	$("button[class*='btn'][id='login']").click(function() {
		var user = $("input[type='text'][name='user']").val();
		var pswd = $("input[type='password'][name='pswd']").val();
		if (user.length==0||pswd.length==0){
			alert("error");
		}
		else{
			var json = JSON.stringify(new LoginBean(user,pswd));
			$.ajax({
				type: "post",
				url: "/OnlineJudge/user/login",
				data: json,
				contentType:'application/json',
				dataType: 'json',
				success: function(data){
					if (data=="success"){
						location.reload();
					}
					else {
						alert(data);
					}
				}
			});
		}
		return false;
	});
	
	$("button[class*='btn'][id='logout']").click(function() {
		$.post("/OnlineJudge/user/logout",function(data){
			if (data=="success"){
				location.reload();
			}
		})
	});
	
	$("button[class*='btn'][id='register']").click(function() {
		$("div[class*='alert'][id='reg-alert']").addClass('hide');
		$("div[id='register-dialog']").modal('show');
		return false;
	});
	
	$("div[class*='btn'][id='dialog-register']").click(function() {
		$("div[class*='alert'][id='reg-alert']").removeClass('alert-error').addClass('alert-info');
		$("div[id='reg-alert-detail']").html("validing...");
		$("div[class*='alert'][id='reg-alert']").removeClass('hide');
		$("div[class*='btn'][id='dialog-register']").button('loading');
		var username = $("#RegUsername").val();
		var password = $("#RegPassword").val();
		var nickname = $("#RegNickname").val();
		var email = $("#RegEmail").val();
		var Check = /^\w[a-zA-Z0-9_]{2,20}$/;
		if (username.length==0||password.length==0||nickname.length==0){
			$("div[class*='alert'][id='reg-alert']").removeClass('alert-info').addClass('alert-error');
			$("div[id='reg-alert-detail']").html("input error");
			$("div[class*='btn'][id='dialog-register']").button('reset');
		}
		else if (!Check.test(username)){
			$("div[class*='alert'][id='reg-alert']").removeClass('alert-info').addClass('alert-error');
			$("div[id='reg-alert-detail']").html("username should be word or number");
			$("div[class*='btn'][id='dialog-register']").button('reset');
		}
		else{
			var json = JSON.stringify(new RegistBean(username,password,nickname,email));
			$.ajax({
				type: "post",
				url: "/OnlineJudge/user/register",
				data: json,
				contentType:'application/json',
				dataType: 'json',
				success: function(data){
					if (data=="success"){
						$("div[class*='alert'][id='reg-alert']").removeClass('alert-info').addClass('alert-success');
						$("div[id='reg-alert-detail']").html("success");
						location.reload();
					}
					else {
						$("div[class*='alert'][id='reg-alert']").removeClass('alert-info').addClass('alert-error');
						$("div[id='reg-alert-detail']").html(data);
					}
					$("div[class*='btn'][id='dialog-register']").button('reset');
				}
			});
		}
	});
});