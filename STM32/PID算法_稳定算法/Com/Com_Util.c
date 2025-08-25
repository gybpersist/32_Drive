PID_Info pid;  // 需要传入参数 desired measure ,并设置 Kd Ki Kp

//  PID 参数
// PID_Info pid = {.Kd = 0.09f, .Ki = 2.2f, .Kp = 0.1f};

void Com_Util_PID(PID_Info *pid)
{
    // 计算误差
    float error = pid->desired - pid->measure;

    // 计算积分项
    pid->integral += error;

    // 计算结果
    pid->result = (pid->Kp * error) + (pid->Ki * pid->integral * pid->dt) + pid->Kd * (error - pid->last_error) / pid->dt;

    // 保存当前误差
    pid->last_error = error;
}