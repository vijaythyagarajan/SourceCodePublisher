/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs : WPF Client that uses Translator to call native C++ code  //
// ver 1.0                                                                 //
// Application    : Project 3 GUI Implementation                           //
// Platform       : Visual Studio 2017 Community Edition                   //
//                                                                         //
// Author         : Vijay Thyagarajan, Syracuse University                 //
/////////////////////////////////////////////////////////////////////////////
/*
 *  Package Decription:
 * =====================
 *  This package defines the GUI using MainWindow.xaml file which is compiled
 *  to a working GUI saving you the effort of programatically creating GUIs.
 *  This package depends on Translator project, if Translator project is NOT built then
 *  Translator.dll does not exist and therefore you'll get Intellisense error saying
 *  you have missing reference. Do not worry about the error, once Translator project
 *  is successfully built this will build and run.
 *  
 *  Public Interface: N/A
 *  
 *  Required Files:
 * =================
 *  MainWindow.xaml MainWindow.xaml.cs Translator.dll
 *  
 *  Build Process:
 * ================
 *  msbuild WPF.csproj
 *  
 *  Maintainence History:
 * =======================
 *  ver 1.0 - April 5th 2019
 *   - first release
 *  ver 2.0 - April 20th 2019
 *  Status bar is added to display message passed between Client and server 
 *  Added methods to support demostration
 * 
 */

using System;
using System.Windows;
using System.Windows.Input;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Controls;
using System.IO;
using System.Windows.Media.Imaging;
using System.Threading.Tasks;
using System.Threading;
using MsgPassingCommunication;
using System.Text.RegularExpressions;
namespace WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // public helper class for the custom listbox in nav and diaplay view
        String workingDir = @"../../../ClientDirectory";
        public bool isShown { get; set; }
        public class listboxItem
        {
            // fileType is an enum to describe the type of file like html, directory or the file
            enum fileType { file, directory, html };
            // listbox selection item class constructor
            public listboxItem(string value, Image img, String actualPath, int fileType) { Str = value; Image = img; ActualPath = actualPath; selectionType = fileType; }
            public string Str { get; set; }
            public string ActualPath { get; set; }
            public Image Image { get; set; }
            public int selectionType { get; set; }
        }

        public string path { get; set; }
        public string displayName { get; set; }

        // -----< MainWindow constructor >---------------------------
        public MainWindow()
        {
            InitializeComponent();
        }

        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        CsEndPoint serverEndPoint;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    if (msg.attributes.Count > 0)
                    {
                        string msgId = msg.value("command");
                        if (dispatcher_.ContainsKey(msgId))
                            dispatcher_[msgId].Invoke(msg);
                    }
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        // -----< Window loaded event handler>-------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            
            string[] args = Environment.GetCommandLineArgs();
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            if (args.Length == 1)
            {
                Console.Write("endpoint_test no ");
                endPoint_.port = 8082;
            }
            else
            {
                endPoint_.port = Int32.Parse(args[1]);
                int len = Int32.Parse(args[1]);
                this.Title = "Source Publisher : " + len;
            }
            translater = new Translater();
            workingDir += endPoint_.machineAddress + endPoint_.port;
            translater.listen(endPoint_, workingDir, workingDir);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();

            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = Int32.Parse(args[2]);
            Path.Text = "Storage";
            pathStack_.Push("..");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }

        
        // -----< Send button click event handler >-----------------------------------
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            
        }

      //  <---------- <Updating the nav list with names for directories and files >--------
        private void updateDirectoryFilesList()
        {
            if (endPoint_ != null && pathStack_.Count > 0)
            {
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "getDirs");
                msg.add("path", pathStack_.Peek());
                translater.postMessage(msg);
                msg.remove("command");
                msg.add("command", "getFiles");
                translater.postMessage(msg);
            }
           
        }
        //<----------------- event to trigger when the listbox item id double clicked in the nav display >-----------------
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            listboxItem ListboxInstance = ((listboxItem)lstBoxFilesForCheckin.SelectedItem);
            string selectedDir = ListboxInstance.Str;
            if (ListboxInstance.selectionType == 1)
            {
                {
                  
                    if (selectedDir == "• •")
                    {
                        if (pathStack_.Count > 1)  // don't pop off "Storage"
                            pathStack_.Pop();
                        else
                            return;
                    }
                    else
                    {
                        path = pathStack_.Peek() + "/" + selectedDir;
                        pathStack_.Push(path);
                    }
                    // display path in Dir TextBlcok
                    Path.Text = removeFirstDir(pathStack_.Peek());

                    // build message to get dirs and post it
                    CsEndPoint serverEndPoint = new CsEndPoint();
                    serverEndPoint.machineAddress = "localhost";
                    serverEndPoint.port = 8080;
                    CsMessage msg = new CsMessage();
                    msg.add("to", CsEndPoint.toString(serverEndPoint));
                    msg.add("from", CsEndPoint.toString(endPoint_));
                    msg.add("command", "getDirs");
                    msg.add("path", pathStack_.Peek());
                    translater.postMessage(msg);

                    // build message to get files and post it
                    msg.remove("command");
                    msg.add("command", "getFiles");
                    translater.postMessage(msg);

                    statusBarText.Text = @"Sending Message from Client" + @"(" + msg.attributes["from"] + ") to Server : " + @"( " + msg.attributes["to"] + ")" + @" with Command : " + "getDirs " + msg.attributes["command"];
                }
            }
        }


        // -----< Window closed event handler - finialize your stuff here >-----------
        private void Window_Closed(object sender, EventArgs e)
        {
            Console.Write(@"window closed");
        }

        //<----------- browse button click >-----------------------------------------------
        private void browseButton(object sender, RoutedEventArgs e)
        {
            Console.Write(@"The browse button is clicked to select files and directories");
            Console.Write(Environment.NewLine);
            System.Windows.Forms.FolderBrowserDialog dialog;
            dialog = new System.Windows.Forms.FolderBrowserDialog();
            dialog.SelectedPath = "..";
            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Path.Text = dialog.SelectedPath;
                path = Path.Text;
                pathStack_.Push(path);
                updateDirectoryFilesList();

            }
        }
        //<--------------- upon publishcodde button click>-------------------------------------
        private void publisherCodeButton(object sender, RoutedEventArgs e)
        {
            Console.Write(@"The Publish Code button is pressed with following arguments"); Console.Write(Environment.NewLine);
            Console.Write("path : "); Console.WriteLine(path); Console.Write(Environment.NewLine);
            Console.Write("Pattern : "); Console.WriteLine(pattern.Text); Console.Write(Environment.NewLine);
            Console.Write("regex : "); Console.WriteLine(regex.Text); Console.Write(Environment.NewLine);

            Console.Write(@"Commandline arguments are passed from the GUI to the publisher Package"); Console.Write(Environment.NewLine);

            Console.Write(Environment.NewLine);

            display_view.Items.Clear();
            string optionValue = null;
            if (option.IsChecked == true)
                optionValue = "/s";
            else
                optionValue = "";

            //convertFiles

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "convertFiles");
            msg.add("path", pathStack_.Peek());
            msg.add("option", optionValue);
            msg.add("regex", regex.Text);
            msg.add("pattern", pattern.Text);
            translater.postMessage(msg);
            statusBarText.Text = @"Sending Message from Client" + @"(" + msg.attributes["from"] + ") to Server : " + @"( " + msg.attributes["to"] + ")" + @" with Command : " + "getDirs " + msg.attributes["command"];



        }
        //<------------ Event to execute on selected item in the display view listbox >----------------------------
        private void Display_view_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            ListBox ListboxType = sender as ListBox;
            if (ListboxType != null)
            {
                listboxItem listboxItemInstance = ((listboxItem)display_view.SelectedItem);
                FileInfo displayFileName = new FileInfo(listboxItemInstance.Str.ToString());

                String fileToProcess = displayFileName.FullName;

                CsMessage msgf = new CsMessage();
                msgf.add("to", CsEndPoint.toString(serverEndPoint));
                msgf.add("from", CsEndPoint.toString(endPoint_));
                msgf.add("command", "getfile");
                msgf.add("name", listboxItemInstance.Str.ToString());
                translater.postMessage(msgf);
                statusBarText.Text = @"Sending Message from Client" + @"(" + msgf.attributes["from"] + ") to Server : " + @"( " + msgf.attributes["to"] + ")" + @" with Command : " + msgf.attributes["command"];

            }

        }
        //---------------< Update the path of the directory under consideration for convertion >---------------------
        private void Path_TextChanged(object sender, TextChangedEventArgs e)
        {
            path = Path.Text;
            if (path == "..") Path.Text = @"Storage";
            updateDirectoryFilesList();
        }

        // -----< to clear the directory list >-----------------------------------
        private void clearDirs()
        {
            lstBoxFilesForCheckin.Items.Clear();
        }

        // -----< to clear the converted files list  >-----------------------------------
        private void clearDisplayView()
        {
            display_view.Items.Clear();
        }

        // -----< to display file on the browser >-----------------------------------
        private void showFileOnBrowser(string fileName)
        {
            this.Dispatcher.Invoke(() =>
            {
                tabControl.SelectedIndex = 1;
            });
            
            string file = Regex.Replace(fileName, ".html", String.Empty);
            String searchPath = workingDir + "/" + fileName;
            FileInfo f = new FileInfo(searchPath);
            string fullname = f.FullName;
            Process.Start(fullname);
        }

        //----< function dispatched by child thread to main thread >-------

        private void addConvertedFiles(string convertedFile)
        {
            Image DisplaylistBoxImage = new Image();
            String filePath = Directory.GetCurrentDirectory();
            filePath += @"/abc.png";
            DisplaylistBoxImage.Source = new BitmapImage(new Uri(filePath));
            string name = System.IO.Path.GetFileName(convertedFile);
            display_view.Items.Add(new listboxItem(name, DisplaylistBoxImage, convertedFile, 2));
            displayName = convertedFile;
        }

        // -----< add directories to the listbox >-----------------------------------
        private void addDir(string dir)
        {
            if (lstBoxFilesForCheckin.Items.Count < 1)
                insertParent();
            String filePath = Directory.GetCurrentDirectory();
            Image displayDirImage = new Image();
            filePath += @"/download.png";
            displayDirImage.Source = new BitmapImage(new Uri(filePath));
            lstBoxFilesForCheckin.Items.Add(new listboxItem(dir, displayDirImage, dir, 1));
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            lstBoxFilesForCheckin.Items.Add(new listboxItem("• •", null, "..", 1));
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            // lstBoxFilesForCheckin.Items.Clear();
        }
 

        // -----< Addd files name to the listboc >-----------------------------------
        private void addFile(string file)
        {
            if (lstBoxFilesForCheckin.Items.Count < 1)
                insertParent();
            String filePath = Directory.GetCurrentDirectory();
            Image displayDirImage = new Image();
            filePath += @"/download.png";
            displayDirImage.Source = new BitmapImage(new Uri(filePath));
            lstBoxFilesForCheckin.Items.Add(new listboxItem(file, displayDirImage, file, 2));
        }
        //----< add client processing for message with key >---------------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------

        private void DispatcherGetFile()
        {
            Action<CsMessage> getFile = (CsMessage rvrMsg) =>
            {
                var enumer = rvrMsg.attributes.GetEnumerator();
                int length = System.Convert.ToInt32(rvrMsg.attributes["content-length"]);
                if (length > 0)
                {
                    while (enumer.MoveNext())
                    {
                        string key = enumer.Current.Key;
                        if (key.Contains("file"))
                        {
                            showFileOnBrowser(rvrMsg.attributes["file"]);
                        }

                    }
                }
            };
            addClientProc("getfile", getFile);
        }

        //----< load converFiles processing into dispatcher dictionary >-------
        private void DispatcherLoadGetConvertedFiles()
        {
            Action<CsMessage> getConvertedFiles = (CsMessage rvrMsg) =>
            {
                Action clrDisplayView = () =>
                {
                    clearDisplayView();
                };
                Dispatcher.Invoke(clrDisplayView, new Object[] { });
                var enumer = rvrMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("convertFiles"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addConvertedFiles(dir);
                            statusBarText.Text = @"Files are converted and stored in convertedPages to view tap on the DisplayView and select a file to view";
                           
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                        this.Dispatcher.Invoke(() =>
                        {
                            if (!isShown)
                            {
                                RaiseSelectionChanged(display_view);
                                isShown = true;
                            }

                        });
                    }
                }
            };
            addClientProc("convertFiles", getConvertedFiles);
        }

        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    //  insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                            statusBarText.Text = @" List of Directories and Files received from " + rcvMsg.attributes["from"];
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });

                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }
        //----< load all dispatcher processing >---------------------------

        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherGetFile();
            DispatcherLoadGetConvertedFiles();
        }

        //----< to remove the first directory in the path >-------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        //--------------------< test function to publish the code for demonstration >--------
        void publishCode(string topicName)
        {
            path = pathStack_.Peek() + topicName;
            pathStack_.Push(path);
            Path.Text = removeFirstDir(pathStack_.Peek());
            Path.Text = topicName;
            pattern.Text = "*.h *.cpp";
            regex.Text = "[A-H](.*)";
            Console.Write(@"The Publish Code button is pressed with following arguments"); Console.Write(Environment.NewLine);
            Console.Write("path : "); Console.WriteLine(path); Console.Write(Environment.NewLine);
            Console.Write("Pattern : "); Console.WriteLine(pattern.Text); Console.Write(Environment.NewLine);
            Console.Write("regex : "); Console.WriteLine(regex.Text); Console.Write(Environment.NewLine);

            Console.WriteLine(@"Commandline arguments are passed from the GUI to the publisher Package"); Console.Write(Environment.NewLine);

            Console.WriteLine(Environment.NewLine);

            display_view.Items.Clear();
            Console.Write(Environment.NewLine);
            string optionValue = null;
            if (option.IsChecked == true)
                optionValue = "/s";
            else
                optionValue = "";

            optionValue = "/s";
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "convertFiles");
            msg.add("path", pathStack_.Peek());
            msg.add("option", optionValue);
            msg.add("regex", regex.Text);
            msg.add("pattern", pattern.Text);
            translater.postMessage(msg);
            Console.WriteLine("Requirement 6: Convert Message is published");
            Console.WriteLine(Environment.NewLine);
            Console.WriteLine("-----------------------------------------------");
            Console.WriteLine(Environment.NewLine);


            statusBarText.Text = @"Sending Message from Client" + @"(" + msg.attributes["from"] + ") to Server : " + @"( " + msg.attributes["to"] + ")" + @" with Command : " + "getDirs " + msg.attributes["command"];
            Thread.Sleep(10);
        }

        //----< function triggered when there is a selection in the display_view listbox >-------
        void PrintText(object sender, SelectionChangedEventArgs args)
        {
            ListBox ListboxType = sender as ListBox;
            if (ListboxType != null)
            {
                listboxItem listboxItemInstance = ((listboxItem)display_view.SelectedItem);
                FileInfo displayFileName = new FileInfo(listboxItemInstance.Str.ToString());

                String fileToProcess = displayFileName.FullName;

                CsMessage msgf = new CsMessage();
                msgf.add("to", CsEndPoint.toString(serverEndPoint));
                msgf.add("from", CsEndPoint.toString(endPoint_));
                msgf.add("command", "getfile");
                msgf.add("name", listboxItemInstance.Str.ToString());
                translater.postMessage(msgf);
                statusBarText.Text = @"Sending Message from Client" + @"(" + msgf.attributes["from"] + ") to Server : " + @"( " + msgf.attributes["to"] + ")" + @" with Command : " + msgf.attributes["command"];

            }

        }

        //----< To post a getfile message for test demonstration >-------
        void showDisplayView(string fileName)
        {
            if(display_view.Items.Count > 0)
            {
                
                CsMessage msgf = new CsMessage();
                msgf.add("to", CsEndPoint.toString(serverEndPoint));
                msgf.add("from", CsEndPoint.toString(endPoint_));
                msgf.add("command", "getfile");
                msgf.add("name",fileName);
                translater.postMessage(msgf);
                statusBarText.Text = @"Sending Message from Client" + @"(" + msgf.attributes["from"] + ") to Server : " + @"( " + msgf.attributes["to"] + ")" + @" with Command : " + msgf.attributes["command"];

            }
        }

        //----< Actions to perform when the WPF Application window is rendered >-------
        //----< demonstartion strarts if the command line argument has a key word "DEMO" >-------
        private void Window_ContentRendered(object sender, EventArgs e)
        {
            Console.WriteLine(" Requirement 3: A WPF Application is presented for the Client", Environment.NewLine);

            Console.WriteLine("============================================================", Environment.NewLine);
            string[] args = Environment.GetCommandLineArgs();
            Console.WriteLine("The command line argument passed for the client is");

            foreach(var part in args)
            {
                Console.Write(part);
                Console.Write("  ");
                Console.WriteLine(Environment.NewLine);
            }

            Console.WriteLine("============================================================", Environment.NewLine);
            string[] topics = new string[3];
            topics[0] = "/DirectoryNavigator";
            topics[1] = "/DepedencyAnalyser";
            topics[2] = "/CppParser";
            Random rnd = new Random();
            string topicName = topics[rnd.Next(0, 2)];
            isShown = true;
            if (args.Length > 3)
            {
                foreach (var command in args)
                {
                    if (command == "DEMO")
                    {
                        Console.WriteLine("The client is running in a Demonstration mode",Environment.NewLine);
                        Console.WriteLine("-----------------------------------------------------------------", Environment.NewLine);

                        isShown = false;
                        Application.Current.Dispatcher.Invoke(new Action(() => { publishCode(topicName); }));
                    }
             
                }
        
            }
        }

        //----< To raise an listbox selection event >-------
        public static void RaiseSelectionChanged(ListBox cbx)
        {
            int index = cbx.SelectedIndex;
            cbx.SelectedIndex = 1;
            cbx.SelectedIndex = 1;
        }

        //----< tab control selection changed >-------
        private void OnSelectionChanged(Object sender, SelectionChangedEventArgs args)
        {
            var tc = sender as TabControl; 

            if (tc != null)
            {
                if(tc.SelectedIndex == 1)
                {
                    statusBarText.Text = "Select a file from the list to view it on the browser";
                }
                else
                {
                    statusBarText.Text = "Select a Directory with Regex ,Pattern  and click on Publish Code button";

                }
            }
        }
    }
}
