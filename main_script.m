clear all
close all
clc

run traces.m

myvars = who;

varToBeAvoided = "time";

%% plot each quantity individually
for i = 1:length(myvars)
    varname = string(myvars(i));
    if( strcmp ( varname , varToBeAvoided ) == 0 )
        figure
        stairs(time, eval(varname), 'LineWidth', 2, 'DisplayName',varname)
        legend
    end
end 

%% plot all quantities together
figure
for i = 1:length(myvars)
    varname = string(myvars(i));
    if( strcmp ( varname , varToBeAvoided ) == 0 )
        stairs(time, eval(varname), 'LineWidth', 2, 'DisplayName',varname)
        hold on
        % title(varname)
    end
end 
legend

%% plot balance of power for the system and SOC
% (i.e., power drained from battery, in red, and supplied to battery, in green)

if( exist('Psensor2','var') == 1)
    PtotCons = Psensor1 + Psensor2 + Pmcu + Prf;
else 
    PtotCons = Psensor1;
end

figure
stairs(time,PtotCons,'linewidth',2,'color','r', 'DisplayName','Ptot consumed')
hold on
stairs(time,Ppv,'linewidth',2,'color','g', 'DisplayName','Ppv')
hold on
stairs(time,SOC,'linewidth',3,'color','b', 'DisplayName','SOC')
legend


% 
% figure Name 'each of the load powers vs total power harvested'
% % stairs(time,SOC,'linewidth',2,'color','b', 'DisplayName','SOC')
% grid on
% % LOADS
% hold on
% stairs(time,Psensor1,'linewidth',2,'color','r', 'DisplayName','Psensor1')
% hold on
% stairs(time,Psensor2,'linewidth',2,'color','c', 'DisplayName','Psensor2')
% hold on
% stairs(time,Prf,'linewidth',2,'color','y', 'DisplayName','Prf')
% hold on
% stairs(time,Pmcu,'linewidth',2,'color','b', 'DisplayName','Pmcu')
% 
% % PV MODULE
% hold on
% stairs(time,Ppv,'linewidth',2,'color','g', 'DisplayName','Ppv')
% 
% %xticks(0:1440:86390)
% %xticklabels({'day 1', 'day 2','day 3', 'day 4', 'day 5', 'day 6' , 'day 7', 'day 8', 'day 9','day 10', 'day 11', 'day 12','day 13', 'day 14', 'day 15', 'day 16' , 'day 17', 'day 18', 'day 19', 'day 20', 'day 21', 'day 22','day 23', 'day 24', 'day 25', 'day 26' , 'day 27', 'day 28', 'day 29', 'day 30', 'day 31', 'day 32','day 33', 'day 34', 'day 35', 'day 36' , 'day 37', 'day 38', 'day 39', 'day 40', 'day 41', 'day 42','day 43', 'day 44', 'day 45', 'day 46' , 'day 47', 'day 48', 'day 49', 'day 50', 'day 51', 'day 52','day 53', 'day 54', 'day 55', 'day 56' , 'day 57', 'day 58', 'day 59', 'day 60'})
% 
% figure Name 'total power consumed vs total power harvested'
% % TOTAL POWER LOADS
% PtotalCons = Psensor1 + Psensor2 + Pmcu + Prf;
% stairs(time,PtotalCons,'linewidth',2,'color','r', 'DisplayName','Ptotal')
% hold on
% 
% % TOTAL POWER SUPPLIED
% stairs(time,Ppv,'linewidth',2,'color','g', 'DisplayName','Ppv')
% 
% xticks(0:1440:86390)
% xticklabels({'day 1', 'day 2','day 3', 'day 4', 'day 5', 'day 6' , 'day 7', 'day 8', 'day 9','day 10', 'day 11', 'day 12','day 13', 'day 14', 'day 15', 'day 16' , 'day 17', 'day 18', 'day 19', 'day 20', 'day 21', 'day 22','day 23', 'day 24', 'day 25', 'day 26' , 'day 27', 'day 28', 'day 29', 'day 30', 'day 31', 'day 32','day 33', 'day 34', 'day 35', 'day 36' , 'day 37', 'day 38', 'day 39', 'day 40', 'day 41', 'day 42','day 43', 'day 44', 'day 45', 'day 46' , 'day 47', 'day 48', 'day 49', 'day 50', 'day 51', 'day 52','day 53', 'day 54', 'day 55', 'day 56' , 'day 57', 'day 58', 'day 59', 'day 60'})
% 
% figure
% subplot(2,1,1)
% stairs(time,SOC,'linewidth',2,'color','m', 'DisplayName','SOC')
% subplot(2,1,2)
% stairs(time,PtotalCons,'linewidth',2,'color','r', 'DisplayName','Ptotal')
% hold on
% stairs(time,Ppv,'linewidth',2,'color','g', 'DisplayName','Ppv')
% 
% figure
% PtotSensors = Psensor1 + Psensor2;
% stairs(time,PtotSensors,'linewidth',2,'color','r', 'DisplayName','PtotalSensors')
% hold on
% stairs(time,Prf,'linewidth',2,'color','m', 'DisplayName','Prf')
% hold on
% stairs(time,Pmcu,'linewidth',2,'color','b', 'DisplayName','Pmcu')
% % PV MODULE
% hold on
% stairs(time,Ppv,'linewidth',2,'color','g', 'DisplayName','Ppv')