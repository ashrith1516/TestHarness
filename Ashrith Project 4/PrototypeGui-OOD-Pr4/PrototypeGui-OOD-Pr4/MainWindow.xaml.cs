/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Prototype for OOD Project #4               //
// ver 1.2                                                         //
// Author : Ashrith Chandramouli                                   //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: left for students
 * - Show Results:  left for students
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs, Translater.h
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 : 12 Dec 2018
 * - Added Translator functionality and integrated comm
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Threading;
using MsgPassingCommunication;
using System.Security.Principal;

namespace PrototypeGui_OOD_Pr4
{
  ///////////////////////////////////////////////////////////////////
  // MainWindow class
  // - Provides navigation and selection to find libraries to test.
  // - Creates a popup window to handle selections.
  
  public partial class MainWindow : Window
  {
    public string path { get; set; }
    List<string> selectedFiles { get; set; } = new List<string>();
    public SelectionWindow swin { get; set; } = null;
    bool unselecting = false;
    private Stack<string> pathStack_ = new Stack<string>();
    private Translater translater;
    private CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_
      = new Dictionary<string, Action<CsMessage>>();
    public MainWindow()
    {
      InitializeComponent();
    }
    //----< open window showing contents of project directory >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            string[] args = Environment.GetCommandLineArgs();
            if (args.Length == 1)
            {
                endPoint_.port = 8080;
            }
            else
            {
                endPoint_.port = Int32.Parse(args[1]);
            }
            translater = new Translater();
            translater.listen(endPoint_);
            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(4, path);
            path += "/TestLibraries";
            LoadNavTab(path);
            if (endPoint_.port == 8080)
            {
                LoadReqs();
                MessageBox.Show("A test request has already been sent for showing the requirements. Please check one of the child processes for the results. It will be available after 10 seconds");
            }
            Results.Items.Clear();
            String resPath = Directory.GetCurrentDirectory();
            resPath = getAncestorPath(4, resPath);
            resPath += "/SaveLibraries";
            string[] files = Directory.GetFiles(resPath);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Results.Items.Add(name);
            }
        }
        //----< find parent paths >--------------------------------------
        private void LoadReqs()
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8082;
            CsMessage fileMsg = new CsMessage();
            fileMsg.add("to", CsEndPoint.toString(serverEndPoint));
            fileMsg.add("from", CsEndPoint.toString(endPoint_));
            fileMsg.add("value", "sendingFile");
            fileMsg.add("name", "ClientDll");
            fileMsg.add("file", "ReqsDemo.dll");
            fileMsg.add("sendingFile", "ReqsDemo.dll");
            string fileWithExt = Directory.GetCurrentDirectory();
            fileWithExt = getAncestorPath(4, fileWithExt);
            fileWithExt += "/TestLibraries/ReqsDemo.dll";
            translater.postMessage(fileMsg);
            string testRequest = "ReqsDemo, Ashrith, " + DateTime.Now.ToString() + ", ReqsDemo.dll";
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "testReq");
            msg.add("name", testRequest);
            translater.postMessage(msg);

        }
            private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        string getAncestorPath(int n, string path)
    {
      for(int i=0; i<n; ++i)
        path = Directory.GetParent(path).FullName;
      return path;
    }
        //----< file Find Libs tab with subdirectories and files >-------

    private void Tab_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //Results.Items.Clear();
            //String resPath = Directory.GetCurrentDirectory();
            //resPath = getAncestorPath(4, resPath);
            //resPath += "/SaveLibraries";
            //string[] files = Directory.GetFiles(resPath);
            //foreach (string file in files)
            //{
            //    string name = System.IO.Path.GetFileName(file);
            //    Results.Items.Add(name);
            //}
        }
    string LoadNavTab(string path)
    {
      Dirs.Items.Clear();
      CurrPath.Text = path;
            try
            {
                string[] dirs = Directory.GetDirectories(path);
                CurrPath.Text = path;
                Dirs.Items.Add("..");
                foreach (string dir in dirs)
                {
                    DirectoryInfo di = new DirectoryInfo(dir);
                    string name = System.IO.Path.GetDirectoryName(dir);
                    Dirs.Items.Add(di.Name);
                }
                string[] files = Directory.GetFiles(path);
                foreach (string file in files)
                {
                    string name = System.IO.Path.GetFileName(file);
                    Dirs.Items.Add(name);
                }
                return path;
            }
            catch (System.IO.IOException)
            {
                string fileName = System.IO.Path.GetFileName(path);
                CurrPath.Text = path;
                string desName = Directory.GetCurrentDirectory();
                desName = getAncestorPath(4, desName);
                desName += @"\TestLibraries\" + fileName;
                try
                {
                    File.Copy(path, desName, true);
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                }
                path = Directory.GetParent(path).FullName;
                string[] dirs = Directory.GetDirectories(path);
                Dirs.Items.Add("..");
                foreach (string dir in dirs)
                {
                    DirectoryInfo di = new DirectoryInfo(dir);
                    string name = System.IO.Path.GetDirectoryName(dir);
                    Dirs.Items.Add(di.Name);
                }
                string[] files = Directory.GetFiles(path);
                foreach (string file in files)
                {
                    string name = System.IO.Path.GetFileName(file);
                    Dirs.Items.Add(name);
                }
                Files.Items.Add(fileName);
                return path;
            }
        }

    private void results_doubleclick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = "";
            try
            {
                selectedFile = Results.SelectedItem.ToString();
            }
            catch(Exception ex)
            {
                string msg = ex.Message;
            }
            String respath = Directory.GetCurrentDirectory();
            respath = getAncestorPath(4, respath);
            respath += "/SaveLibraries";
            try
            {
                selectedFile = System.IO.Path.Combine(respath,selectedFile);
                string contents = File.ReadAllText(selectedFile);
                SelectionWindow popup = new SelectionWindow();
                popup.setMainWindow(this);
                popup.TextOutput.Text = contents;
                popup.Show();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        
}
//----< handle selections in files listbox >---------------------

private void Files_SelectionChanged(
      object sender, SelectionChangedEventArgs e
    )
    {
      if(unselecting)
      {
        unselecting = false;
        return;
      }
      if (swin == null)
      {
        swin = new SelectionWindow();
        swin.setMainWindow(this);
      }
      swin.Show();

      if (e.AddedItems.Count == 0)
        return;
      string selStr = e.AddedItems[0].ToString();
      selStr = System.IO.Path.Combine(path, selStr);
      if (!selectedFiles.Contains(selStr))
      {
        selectedFiles.Add(selStr);
        swin.Add(selStr);
      }
    }
    //----< unselect files called by unloading SelectionWindow >-----

    public void unselectFiles()
    {
      unselecting = true;  // needed to avoid using selection logic
      //selectedFiles.Clear();
      Files.UnselectAll();
    }
    //----< move into double-clicked directory, display contents >---

    private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
            string selectedDir = "";
            try
            {
                selectedDir = Dirs.SelectedItem.ToString();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
            confirmLbl.Visibility = Visibility.Hidden;
            if (selectedDir == "..")
        path = getAncestorPath(1, path);
      else
        path = System.IO.Path.Combine(path, selectedDir);
     path = LoadNavTab(path);
    }
    
        //----< shut down the SelectionWindow if open >------------------
    private void Window_Unloaded(object sender, RoutedEventArgs e)
    {
      try {
        swin.Close();
      }
      catch (Exception ex)
      {
          string msg = ex.Message;
      }
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8082;
      if(IsAdministrator())
      {
           CsMessage endMsg = new CsMessage();
           endMsg.add("to", CsEndPoint.toString(serverEndPoint));
           endMsg.add("from", CsEndPoint.toString(endPoint_));
           endMsg.add("command", "exit");
           translater.postMessage(endMsg);
      }
      

     }

     public bool IsAdministrator()
        {
            // https://stackoverflow.com/questions/11660184/c-sharp-check-if-run-as-administrator
            var identity = WindowsIdentity.GetCurrent();
            var principle = new WindowsPrincipal(identity);
            return principle.IsInRole(WindowsBuiltInRole.Administrator);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8082;
            CsMessage fileMsg = new CsMessage();
            fileMsg.add("to", CsEndPoint.toString(serverEndPoint));
            fileMsg.add("from", CsEndPoint.toString(endPoint_));
            fileMsg.add("value", "sendingFile");
            fileMsg.add("name", "ClientDll");
            string fileWithExt = "";
            string testRequest = "";
            for (int i = Files.Items.Count - 1; i >= 0; i--)
            {
                fileWithExt = Files.Items.GetItemAt(i).ToString();
                testRequest += fileWithExt.Substring(0, fileWithExt.Length - 4);
            }
            testRequest += ", Ashrith, " + DateTime.Now.ToString();
            for (int i = Files.Items.Count - 1; i >= 0; i--)
            {
                fileMsg.add("file", Files.Items.GetItemAt(i).ToString());
                fileMsg.add("sendingFile", Files.Items.GetItemAt(i).ToString());
                translater.postMessage(fileMsg);
                testRequest += ", " + Files.Items.GetItemAt(i).ToString();
                fileMsg.remove("file");
                fileMsg.remove("sendingFile");
            }
                
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "testReq");
            msg.add("name", testRequest);
            translater.postMessage(msg);
            Files.Items.Clear();
            confirmLbl.Visibility = Visibility.Visible;
        }

        private void Button_Refresh(object sender, RoutedEventArgs e)
        {
            Results.Items.Clear();
            String resPath = Directory.GetCurrentDirectory();
            resPath = getAncestorPath(4, resPath);
            resPath += "/SaveLibraries";
            string[] files = Directory.GetFiles(resPath);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Results.Items.Add(name);
            }
        }

    }
}
