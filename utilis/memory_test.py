import sys
import matplotlib.pyplot as plt
import os
import pandas as pd
import re

L3_MEM_BW = re.compile(r'(?P<column>L3 Memory bandwidth for 1 graph run)\s+\:\s+(?P<value>[0-9]+)\s+Bytes\n')
L2_MEM_BW = re.compile(r'(?P<column>L2 Memory bandwidth for 1 graph run)\s+\:\s+(?P<value>[0-9]+)\s+Bytes\n')
KER_ARGS = re.compile(r'(?P<column>Sum of all Kernels arguments size)\s+\:\s+(?P<value>[0-9]+)\s+Bytes\n')
TIL_OH = re.compile(r'(?P<column>Tiling Bandwith overhead)\s+\:\s+(?P<value>[0-9.]+)\s+Move/KerArgSize\n')
L2_MEM_BW_PER = re.compile(r'(?P<column>Percentage of baseline BW for L2)\s+\:\s+(?P<value>[0-9.]+)\s+\%\n')
L3_MEM_BW_PER = re.compile(r'(?P<column>Percentage of baseline BW for L3)\s+\:\s+(?P<value>[0-9.]+)\s+\%\n')
KER_OPS = re.compile(r'(?P<column>Sum of all Kernels operations)\s+\:\s+(?P<value>[0-9]+)\s+Operations\n')
TOT_COEFFS = re.compile(r'(?P<column>Total amount of flash coefficients)\s+\:\s+(?P<value>[0-9]+)\s+Bytes\n')

matches = [L3_MEM_BW, L2_MEM_BW, TIL_OH, KER_ARGS, L2_MEM_BW_PER, L3_MEM_BW_PER, KER_OPS, TOT_COEFFS]

def main():
	L2_dict = {}
	frames = [] 
	for L2_MEM in range(190, 380, 10):
		columns = []
		stream = os.popen('make -f ssd.mk at_model MODEL_L2_MEMORY={} '.format(L2_MEM*1000))


		out_log = stream.readlines()

		#print(output[-24:-12])
		out_dict = {}
		row = []
		for line in out_log:
			for match in matches:
				m = match.search(line)
				if m:
					columns.append(m['column'])
					row.append(float(m['value']))
					continue
		df1 = pd.DataFrame.from_dict({L2_MEM: row}, orient='index', columns=columns)
		frames.append(df1)

	df = pd.concat(frames, sort=False)

	normalized_df=pd.DataFrame()
	for i,j,k in zip(df.min(),df.max(),df.columns):
		normalized_df[k]=(df[k]-(i if(i-j!=0) else 0))/((j-i)if(i-j!=0) else i)
	print(normalized_df)
	normalized_df.cumsum()
	normalized_df.plot()

	plt.xlabel('L2 MEM [KB]')
	plt.ylabel('normalized betw een min-max')

	plt.show()

if __name__ == "__main__":
	main()
