# import useful things
import numpy as np
import sys
import matplotlib.pyplot as plt
import matplotlib.dates
from matplotlib import ticker
from matplotlib.backends.backend_pdf import PdfPages
#from matplotlib.dates import strpdate2num
#from matplotlib.dates import num2date

from pylab import *
from scipy.optimize import leastsq
from scipy.optimize import curve_fit
from datetime import datetime
import argparse

# write a data file to a numpy file, returns data from the txt file
def txtWithDatesToNpy(inputFile, outputFile):
	database = np.loadtxt(inputFile, delimiter="\t", converters={1:strpdate2num('%Y-%m-%d %H:%M:%S')},)
	np.save(outputFile, database, allow_pickle=True)
	return database

# get a desired time period and create an reduced data sample
def reduceToTimePeriod(inputData, minimumValue, maximumValue):
	minimumValue = np.argmax(inputData[:,1]>minimumValue)
	maximumValue = np.argmax(inputData[:,1]>maximumValue)
	if maximumValue == 0: maximumValue = -1
	return inputData[minimumValue:maximumValue,:]

# a linear fit to some data
def fitLinearToData(xData, yData):
	fitfunc  = lambda p, x: p[0]+x*p[1]
	errfunc  = lambda p, x, y: (y - fitfunc(p, x))
	init  = [736983, (9.84e-6)/0.4-(2.94e-6)/0.4]
	
	fitResult = leastsq( errfunc, init, args=(xData, yData))

	return fitResult, fitfunc

# a linear function
def linearFunction(x, b, m):
	return x*m + b

# a constant
def constantFunction(x, b):
	return b

# fit of any function
def fitAFunctionToTheData(function, x, y):
	params, paramsCovariance = curve_fit(function, x, y)
	return params, paramsCovariance 

# formater for times on axis
def formatTimeAxis(x, pos=None):
    return num2date(x).strftime('%Y/%m/%d %H:%M:%S')



# use the above things
if __name__ == "__main__":
	# prepare the import of the data
	parser = argparse.ArgumentParser(description='Display mysql data and fit functions to it')
	parser.add_argument('InputFile', metavar='f', type=str, nargs='?', help='Input file with the data stream exported from a mysql data base without extension')
	parser.add_argument('StartTime', metavar='s', type=str, nargs='?', help='Start time YYYY-MM-DD HH:MM:SS') 
	parser.add_argument('EndTime', metavar='e', type=str, nargs='?', help='End time, YYYY-MM-DD HH:MM:SS')
	parser.add_argument("-c", action='store_true', help='Create the npy database from the text file')
	parser.add_argument("-l", action='store_true', help='Do a linear fit in the time range of interest')
	parser.add_argument("-const", action='store_true', help='Fit a constant in the time range of interest')
	parser.add_argument("-s", action='store_true', help='Print the plot to screen')
	args = parser.parse_args()

	databaseName = args.InputFile
	#action = sys.argv[4]
	
	# create datafile
	if args.c: 
		datbase = txtWithDatesToNpy(databaseName+".txt", databaseName)
		#action = "none"

	# read datafile
	database = np.load(databaseName+".npy")

	# reduce to interesting range
	lowerTimeThreshold = args.StartTime
	upperTimeThreshold = args.EndTime
	startTime = matplotlib.dates.date2num(datetime.strptime(lowerTimeThreshold, '%Y-%m-%d %H:%M:%S'))
	finalTime = matplotlib.dates.date2num(datetime.strptime(upperTimeThreshold, '%Y-%m-%d %H:%M:%S'))
	dataToPlot = reduceToTimePeriod(database, startTime, finalTime)

	# perform a fit to the data
	if args.l:
		fitResult, linearFit = fitLinearToData(dataToPlot[:,1], dataToPlot[:,0])
		# create x - y data to plot the fit as well
		yFitData = linearFit(fitResult[0], dataToPlot[:,1])
		print "Pressure rise in bar per second: %.02E" %(fitResult[0][1]*1e-5)
		print "And in Torr l per second (assuming 850l): %.02E" %(fitResult[0][1]*1e-5*750.062*850)
	elif args.const:
		fitResult, constant = fitAFunctionToTheData(constantFunction, dataToPlot[:,1], dataToPlot[:,0])
		yFitData = constantFunction(dataToPlot[:,1], fitResult[0])
		if (databaseName.find("cdg") > 0): print "Chamber pressure: %.02E (in barG)" %fitResult[0]
		else: print "Chamber pressure: %.02E (in barA)" %fitResult[0]

	# convert time to readble time conversions
	makeTimesReadable = True
	if makeTimesReadable:  transformedTimeValues = matplotlib.dates.num2date(dataToPlot[:,1])
	else: transformedTimeValues = dataToPlot[:,1]

	# do a plot
	# settings to enable latex in captions
	matplotlib.rc('text', usetex = True)
	params = {'text.latex.preamble': [r'\usepackage{siunitx}', r'\usepackage{cmbright}']}
	plt.rcParams.update(params)
	# cosmetics
	plt.figure(figsize=(8,6))
	plt.rc('xtick',labelsize=10+4)
	plt.rc('ytick',labelsize=16+4)
	matplotlib.rcParams.update({'font.size': 20+4})

	plt.scatter(transformedTimeValues, dataToPlot[:,0])
	if args.const: plt.hlines(yFitData, startTime, finalTime, color='red') 
	if args.l: plt.plot(transformedTimeValues, yFitData, color='red')
	plt.title(databaseName.replace("_", "-"), pad=25, fontdict= {'fontsize': 20})
	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
	if (databaseName.find("cdg") > 0): plt.ylabel("Pressure in barG", horizontalalignment="right", y=1.0)
	else: plt.ylabel("Pressure in barA", horizontalalignment="right", y=1.0)

	axis = plt.gca()
	axis.set_ylim([np.min(dataToPlot[:,0])-abs(np.min(dataToPlot[:,0]))*0.25, np.max(dataToPlot[:,0])*1.1])
	if makeTimesReadable: 
		axis.xaxis.set_major_formatter(ticker.FuncFormatter(formatTimeAxis))
		gcf().autofmt_xdate()

	outputPdfDoc = PdfPages(databaseName+"_"+lowerTimeThreshold.replace(' ', "-").replace('/','-').replace(':','-')+"_"+upperTimeThreshold.replace(' ', '-').replace('/', '-').replace(':','-')+".pdf")
	figure = plt.gcf()
	outputPdfDoc.savefig(figure)
	outputPdfDoc.close()

	if args.s: plt.show()
