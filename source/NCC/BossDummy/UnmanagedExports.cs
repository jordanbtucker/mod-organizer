using System;
using System.Collections.Generic;
using System.Text;
using RGiesecke.DllExport;
using System.Runtime.InteropServices;

namespace BossDummy
{
   internal static class UnmanagedExports
   {

       #region Error Handling Functions

       [DllExport("GetLastErrorDetails", CallingConvention = CallingConvention.StdCall)]
//       static UInt32 GetLastErrorDetailsDelegate([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(StringMarshaler), MarshalCookie = "UTF8")] out string details)
       static UInt32 GetLastErrorDetails(out string details)
       {
           details = "";
           return 0;
       }

       #endregion

       #region Lifecycle Management Functions


       [DllExport("CreateBossDb", CallingConvention.StdCall)]
       static UInt32 CreateBossDb(ref IntPtr db, UInt32 clientGame, string dataPath)
       {
           db = new IntPtr(1);
           return 0;
       }

       [DllExport("DestroyBossDb", CallingConvention.StdCall)]
       static void DestroyBossDb(IntPtr db)
       {
       }

       #endregion

       #region Database Loading Functions

       [DllExport("Load", CallingConvention.StdCall)]
       static UInt32 Load(IntPtr db, string masterlistPath, string userlistPath)
       {
           return 0;
       }

       [DllExport("EvalConditionals", CallingConvention.StdCall)]
       static UInt32 EvalConditionals(IntPtr db)
       {
           return 0;
       }

       #endregion

       #region Masterlist Updating

       [DllExport("UpdateMasterlist", CallingConvention.StdCall)]
       static UInt32 UpdateMasterlist(IntPtr db, string masterlistPath)
       {
           return 0;
       }

       #endregion

       #region Plugin Sorting Functions

       [DllExport("SortMods", CallingConvention.StdCall)]
       static UInt32 SortMods(IntPtr db, bool trialOnly, out IntPtr sortedPlugins, out UInt32 listLength, out UInt32 lastRecPos)
       {
           sortedPlugins = new IntPtr(0);
           listLength = 0;
           lastRecPos = 0;
           return 0;
       }

       [DllExport("GetLoadOrder", CallingConvention.StdCall)]
       static UInt32 GetLoadOrder(IntPtr db, out IntPtr plugins, out UInt32 numPlugins)
       {
           plugins = new IntPtr(0);
           numPlugins = 0;
           return 0;
       }

       [DllExport("SetLoadOrder", CallingConvention.StdCall)]
       static UInt32 SetLoadOrder(IntPtr db, IntPtr plugins, UInt32 numPlugins)
       {
           return 0;
       }

       [DllExport("GetActivePlugins", CallingConvention.StdCall)]
       static UInt32 GetActivePlugins(IntPtr db, out IntPtr plugins, out UInt32 numPlugins)
       {
           plugins = new IntPtr(0);
           numPlugins = 0;
           return 0;
       }

       [DllExport("SetActivePlugins", CallingConvention.StdCall)]
       static UInt32 SetActivePlugins(IntPtr db, IntPtr plugins, UInt32 numPlugins)
       {
           return 0;
       }

       [DllExport("GetPluginLoadOrder", CallingConvention.StdCall)]
       static UInt32 GetPluginLoadOrder(IntPtr db, string plugin, out UInt32 index)
       {
           index = 0;
           return 0;
       }

       [DllExport("SetPluginLoadOrder", CallingConvention.StdCall)]
       static UInt32 SetPluginLoadOrder(IntPtr db, string plugin, UInt32 index)
       {
           return 0;
       }

       [DllExport("GetIndexedPlugin", CallingConvention.StdCall)]
       static UInt32 GetIndexedPlugin(IntPtr db, UInt32 index, out string plugin)
       {
           plugin = "";
           return 0;
       }

       [DllExport("SetPluginActive", CallingConvention.StdCall)]
       static UInt32 SetPluginActive(IntPtr db, string plugin, bool active)
       {
           return 0;
       }

       [DllExport("IsPluginActive", CallingConvention.StdCall)]
       static UInt32 IsPluginActive(IntPtr db, string plugin, out bool isActive)
       {
           isActive = false;
           return 0;
       }

       #endregion

       #region Utility Methods

       [DllExport("IsPluginMaster", CallingConvention.StdCall)]
       static UInt32 IsPluginMaster(IntPtr db, string plugin, out bool isMaster)
       {
           isMaster = false;
           return 0;
       }

       #endregion
   }
}
