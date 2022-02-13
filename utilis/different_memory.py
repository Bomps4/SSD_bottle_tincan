import sys
import matplotlib.pyplot as plt
import os
import pandas as pd
import re
import numpy as np
from matplotlib.cm import ScalarMappable
plt.rcParams.update({'font.size': 20})
TIME=re.compile('TOTALTIMEINMICROSECONDS:[0-9]+')
PERFORMANCE=re.compile('S[0-9]+.+:Cycles:[0-9]+,Operations:[0-9]+,.+')
TOTAL=re.compile('Total:Cycles:[0-9]+.+')
columns=['Operation_name','Cycles','Operations','Operations/Cycle']
time_columns=['Memory','Execution_time','Operations/Cycle']

tempi={i:[] for i in time_columns}
for L2_MEM in range(180,390,10):
	if(not(L2_MEM>=220 and L2_MEM<240 )):
		plt.figure(figsize=(21, 9), dpi=160)
		divided={i:[] for i in columns}
		
		stream = os.popen('make -f ssd.mk clean all run platform=gvsoc IMAGE={} MODEL_L2_MEMORY={}'.format(os.path.abspath('../tflite_model/test_1_out.ppm'),L2_MEM*1000))

		out_log = stream.readlines()
		for line in out_log:
			line=line.replace(' ','')
			performance=re.search(PERFOR{'Memory': [180, 190, 200, 210, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370], 'Execution_time': [3002625, 863739, 558166, 567596, 607117, 588348, 604462, 567261, 529693, 532867, 524750, 519135, 512939, 495544, 501282, 503174, 500397, 513886], 'Operations/Cycle': [1.014811, 3.52907, 5.462226, 5.371037, 5.020422, 5.182208, 5.044988, 5.374773, 5.756105, 5.722537, 5.813379, 5.875878, 5.949368, 6.158231, 6.087525, 6.064405, 6.097791, 5.938262]}
MANCE,line)
		
			
			tempo=re.search(TIME,line)
			total=re.search(TOTAL,line)

			if performance:
				obtained=re.split(':|,',performance.group(0))
				for j in range(len(columns)):
					if(j!=0):
						divided[columns[j]].append(obtained[2*j])
					else:
						splitted=obtained[0].split('_')
						divided[columns[j]].append(splitted[0]+'_'+splitted[1] if(len(splitted)!=1) else '')
			
			elif tempo:
				tempi['Memory'].append(L2_MEM)
				tempi['Execution_time'].append(int(re.split(':|,',tempo.group(0))[-1]))
				print("SAREI LA PRINT PRIMA",tempi)
			elif total:

				tempi['Operations/Cycle'].append(float(re.split(':|,',total.group(0))[-1]))
				print("sarei la print",tempi)
			
		df=pd.DataFrame(divided)
		print(df)	
		norm = plt.Normalize(0, np.max(df["Operations/Cycle"].astype(float)))
		cm=plt.cm.viridis(norm(df["Operations/Cycle"].astype(float)))
		plt.xlabel('Layer_name')
		plt.ylabel('Operations (percentage over the total)')
		plt.bar(df['Operation_name'],df['Operations'].astype(int)/np.sum(df['Operations'].astype(int)),color=cm)
		plt.xticks(df['Operation_name'],rotation='vertical')
		sm = ScalarMappable(cmap=plt.cm.viridis, norm=norm)
		sm.set_array([])
		cb=plt.colorbar(sm,label='Operations/Cycle')
		plt.savefig('Ope_per_'+str(L2_MEM)+' kb')
		cb.remove()
		plt.draw()
plt.figure(2,figsize=(16, 9), dpi=160)
print(tempi)
df_tempi=pd.DataFrame(tempi)
norm_time = plt.Normalize(np.min(df_tempi["Operations/Cycle"].astype(float)), np.max(df_tempi["Operations/Cycle"].astype(float)))
cm_time =plt.cm.viridis(norm_time(df_tempi["Operations/Cycle"].astype(float)))



plt.xlabel('Allocated Memory (kb)')
plt.ylabel('Execution_times (microseconds)')
plt.bar(df_tempi['Memory'],df_tempi['Execution_time'].astype(int),color=cm_time)
sm_time = ScalarMappable(cmap=plt.cm.viridis, norm=norm_time)
sm_time.set_array([])
plt.colorbar(sm_time,label='Operations/Cycle')
ax2 = plt.twinx()
ax2.set_xlim((180,380))
plt.plot(df_tempi['Memory'].astype(int), df_tempi['Execution_time'].astype(int))
plt.show()



#{'Memory': [180, 190, 200, 210, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370], 'Execution_time': [3002625, 863739, 558166, 567596, 607117, 588348, 604462, 567261, 529693, 532867, 524750, 519135, 512939, 495544, 501282, 503174, 500397, 513886], 'Operations/Cycle': [1.014811, 3.52907, 5.462226, 5.371037, 5.020422, 5.182208, 5.044988, 5.374773, 5.756105, 5.722537, 5.813379, 5.875878, 5.949368, 6.158231, 6.087525, 6.064405, 6.097791, 5.938262]}








